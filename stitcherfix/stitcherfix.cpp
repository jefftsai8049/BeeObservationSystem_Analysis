/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

//#define SPEEDRUN
#ifdef SPEEDRUN
#include <iostream>
#endif
#include "stitcherfix.h"
#include <omp.h>
#include <iostream>

namespace cv {

	Stitcherfix Stitcherfix::createDefault(bool try_use_gpu)
{
	Stitcherfix stitcher;
    stitcher.setRegistrationResol(0.6);
    stitcher.setSeamEstimationResol(0.05);  //0.1
    stitcher.setCompositingResol(ORIG_RESOL);
    stitcher.setPanoConfidenceThresh(0.3);
    stitcher.setWaveCorrection(true);
    stitcher.setWaveCorrectKind(detail::WAVE_CORRECT_HORIZ);
    stitcher.setFeaturesMatcher(makePtr<detail::BestOf2NearestMatcher>(true)); //May crash on NVIDIA cards(?)
    stitcher.setBundleAdjuster(makePtr<detail::BundleAdjusterRay>());

#ifdef HAVE_CUDA
    if (try_use_gpu && cuda::getCudaEnabledDeviceCount() > 0)
    {
#ifdef HAVE_OPENCV_XFEATURES2D
        stitcher.setFeaturesFinder(makePtr<detail::SurfFeaturesFinderGpu>());
#else
        stitcher.setFeaturesFinder(makePtr<detail::OrbFeaturesFinder>());
#endif
        stitcher.setWarper(makePtr<SphericalWarperGpu>());
        stitcher.setSeamFinder(makePtr<detail::GraphCutSeamFinderGpu>());
    }
    else
#endif
    {
#ifdef HAVE_OPENCV_XFEATURES2D
        stitcher.setFeaturesFinder(makePtr<detail::SurfFeaturesFinder>());
#else
        stitcher.setFeaturesFinder(makePtr<detail::OrbFeaturesFinder>());
#endif
        stitcher.setWarper(makePtr<SphericalWarper>());
        stitcher.setSeamFinder(makePtr<detail::DpSeamFinder>());
        stitcher.isCuda = false;
        //stitcher.setSeamFinder(makePtr<detail::GraphCutSeamFinder>(detail::GraphCutSeamFinderBase::COST_COLOR));
    }

    stitcher.setExposureCompensator(makePtr<detail::NoExposureCompensator>());
    //stitcher.setExposureCompensator(makePtr<detail::BlocksGainCompensator>());

    stitcher.setBlender(makePtr<detail::Blender>());
    //stitcher.setBlender(makePtr<detail::MultiBandBlender>(true));

    return stitcher;
}


Stitcherfix::Status Stitcherfix::estimateTransform(InputArrayOfArrays images)
{
    return estimateTransform(images, std::vector<std::vector<Rect> >());
}


Stitcherfix::Status Stitcherfix::estimateTransform(InputArrayOfArrays images, const std::vector<std::vector<Rect> > &rois)
{
    is_first = true;
    images.getUMatVector(imgs_);

    rois_ = rois;

    Status status;

    if ((status = matchImages()) != OK)
        return status;

    if ((status = estimateCameraParams()) != OK)
        return status;

    return OK;
}



Stitcherfix::Status Stitcherfix::composePanorama(OutputArray pano)
{
    return composePanorama(std::vector<UMat>(), pano);
}

/*!
  Increase the speed by using OpenMP and remove some duplicated calculations.\n
  Variables corners. sizes, dst_roi, xmap, ymap, masks of panorama is moved to private
  for further use.
*/
Stitcherfix::Status Stitcherfix::composePanorama(InputArrayOfArrays images, OutputArray pano)
{
    //one-time use fix//
            std::vector<detail::CameraParams> cameras_;
            double warped_image_scale_;
            warped_image_scale_ = warped_image_scale_g;
            cameras_ = cameras_g;
            //one-time use fix//

            //LOGLN("Warping images (auxiliary)... ");

            std::vector<UMat> imgs;
            images.getUMatVector(imgs);
            if (!imgs.empty())
            {
                CV_Assert(imgs.size() == imgs_.size());

                UMat img;
                seam_est_imgs_.resize(imgs.size());

                for (size_t i = 0; i < imgs.size(); ++i)
                {
                    imgs_[i] = imgs[i];
                    resize(imgs[i], img, Size(), seam_scale_, seam_scale_);
                    seam_est_imgs_[i] = img.clone();
                }

                std::vector<UMat> seam_est_imgs_subset;
                std::vector<UMat> imgs_subset;

                for (size_t i = 0; i < indices_.size(); ++i)
                {
                    imgs_subset.push_back(imgs_[indices_[i]]);
                    seam_est_imgs_subset.push_back(seam_est_imgs_[indices_[i]]);
                }

                seam_est_imgs_ = seam_est_imgs_subset;
                imgs_ = imgs_subset;
            }

            //UMat pano_;

    #if ENABLE_LOG
            int64 t = getTickCount();
    #endif
            if (is_first)
            {
                //seam warping maps
                corners_seam.resize(imgs_.size());
                masks_warped.resize(imgs_.size());
                dst_roi_seam.resize(imgs_.size());
                uxmap_seam.resize(imgs_.size());
                uymap_seam.resize(imgs_.size());
                dxmap_seam.resize(imgs_.size());
                dymap_seam.resize(imgs_.size());

                //warping maps
                corners.resize(imgs_.size());
                sizes.resize(imgs_.size());
                w.resize(imgs_.size());
                dst_roi.resize(imgs_.size());
                uxmap.resize(imgs_.size());
                uymap.resize(imgs_.size());
                d_xmap.resize(imgs_.size());
                d_ymap.resize(imgs_.size());

                mask_warped.resize(imgs_.size());
            }

            std::vector<UMat> images_warped(imgs_.size());
            std::vector<UMat> masks(imgs_.size());

            // Prepare image masks
    //#pragma omp parallel for
            for (int64 i = 0; i < imgs_.size(); ++i)
            {
                masks[i].create(seam_est_imgs_[i].size(), CV_8U);
                masks[i].setTo(Scalar::all(255));
            }

            // Warp images and their masks

            std::vector<UMat> images_warped_f(imgs_.size());
            //Use openMP
    #ifdef SPEEDRUN
            int64 t = cv::getTickCount();
    #endif
            if (is_first)
            for (int64 i = 0; i < imgs_.size(); ++i)
            {
                w[i] = warper_->create(float(warped_image_scale_ * seam_work_aspect_));
            }
    //#pragma omp parallel for firstprivate(t)
            for (int64 i = 0; i < imgs_.size(); ++i)
            {
                Mat_<float> K;
                //Remove duplicated calculations
                if (is_first)
                {
                    //w[i] = warper_->create(float(warped_image_scale_ * seam_work_aspect_));
                    cameras_[i].K().convertTo(K, CV_32F);
                    K(0, 0) *= (float)seam_work_aspect_;
                    K(0, 2) *= (float)seam_work_aspect_;
                    K(1, 1) *= (float)seam_work_aspect_;
                    K(1, 2) *= (float)seam_work_aspect_;

                    //corners_seam[i] = w[i]->warp(seam_est_imgs_[i], K, cameras_[i].R, INTER_LINEAR, BORDER_CONSTANT, images_warped[i]);
                    dst_roi_seam[i] = w[i]->buildMaps(seam_est_imgs_[i].size(), K, cameras_[i].R, uxmap_seam[i], uymap_seam[i]);
//                    if (isCuda)
//                    {
//                        dxmap_seam[i].upload(uxmap_seam[i]);
//                        dymap_seam[i].upload(uymap_seam[i]);
//                    }

                    corners_seam[i] = dst_roi_seam[i].tl();
    #ifdef SPEEDRUN
                    std::cout << "First seam additional buildmap compute time: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec" << std::endl;
    #endif
                }
                images_warped[i].create(dst_roi_seam[i].height + 1, dst_roi_seam[i].width + 1, seam_est_imgs_[i].type());
//                if (isCuda)
//                {
//                    d_src.upload(seam_est_imgs_[i]);
//                    cuda::remap(d_src, d_dst, dxmap_seam[i], dymap_seam[i], INTER_LINEAR, BORDER_CONSTANT);
//                    d_dst.download(images_warped[i]);
//                }
//                else

                    cv::remap(seam_est_imgs_[i], images_warped[i], uxmap_seam[i], uymap_seam[i], INTER_LINEAR, BORDER_CONSTANT);


                //Remove duplicated calculations
                if (is_first)
                {
    #ifdef SPEEDRUN
                    int64 t = cv::getTickCount();
    #endif
                    //sizes_seam[i] = images_warped[i].size(); //currently no use
                    w[i]->warp(masks[i], K, cameras_[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);
    #ifdef SPEEDRUN
                    std::cout << "First seam additional warping compute time: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec" << std::endl;
    #endif
                }
                images_warped[i].convertTo(images_warped_f[i], CV_32F);
            }

    #ifdef SPEEDRUN
                t = cv::getTickCount();
    #endif
            // Find seams
            exposure_comp_->feed(corners_seam, images_warped, masks_warped);
    #ifdef SPEEDRUN
            std::cout << "exposure compute time: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec" << std::endl;
    #endif
    #ifdef SPEEDRUN
            t = cv::getTickCount();
    #endif
            seam_finder_->find(images_warped_f, corners_seam, masks_warped);
    #ifdef SPEEDRUN
            std::cout << "Seam time: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec" << std::endl;
    #endif
            // Release unused memory
            seam_est_imgs_.clear();
            images_warped.clear();
            images_warped_f.clear();
            masks.clear();

            //LOGLN("Compositing...");
    #if ENABLE_LOG
            t = getTickCount();
    #endif


            //double compose_seam_aspect = 1;
            double compose_work_aspect = 1;
            bool is_blender_prepared = false;

            double compose_scale = 1;
            bool is_compose_scale_set = false;

    #ifdef SPEEDRUN
            t = cv::getTickCount();
    #endif

            if (!is_compose_scale_set && is_first)
            {
                if (compose_resol_ > 0)
                    compose_scale = std::min(1.0, std::sqrt(compose_resol_ * 1e6 / imgs_[0].size().area()));
                is_compose_scale_set = true;

                // Compute relative scales
                //compose_seam_aspect = compose_scale / seam_scale_;
                compose_work_aspect = compose_scale / work_scale_;

                // Update warped image scale


                warped_image_scale_ *= static_cast<float>(compose_work_aspect);

                // Update corners and sizes
    #pragma omp parallel for
                for (int64 i = 0; i < imgs_.size(); ++i)
                {
                    w[i] = warper_->create((float)warped_image_scale_);
                    // Update intrinsics
                    cameras_[i].focal *= compose_work_aspect;
                    cameras_[i].ppx *= compose_work_aspect;
                    cameras_[i].ppy *= compose_work_aspect;

                    // Update corner and size
                    Size sz = full_img_sizes_[i];
                    if (std::abs(compose_scale - 1) > 1e-1)
                    {
                        sz.width = cvRound(full_img_sizes_[i].width * compose_scale);
                        sz.height = cvRound(full_img_sizes_[i].height * compose_scale);
                    }

                    Mat K;
                    cameras_[i].K().convertTo(K, CV_32F);
                    Rect roi = w[i]->warpRoi(sz, K, cameras_[i].R);
                    corners[i] = roi.tl();
                    sizes[i] = roi.size();
                }
            }
    #ifdef SPEEDRUN
            std::cout << "First camera parameters additional scales update: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec" << std::endl;
    #endif

            if (!is_blender_prepared)
            {
                blender_->prepare(corners, sizes);
                is_blender_prepared = true;
            }

            std::vector<UMat> img_warped_s(imgs_.size());
    //#pragma omp parallel for firstprivate(t)
            for (int64 img_idx = 0; img_idx < imgs_.size(); ++img_idx)
            {
                UMat full_img, img;
                UMat img_warped;
                UMat dilated_mask, seam_mask, mask;

    #if ENABLE_LOG
                int64 compositing_t = getTickCount();
    #endif

                // Read image and resize it if necessary
                full_img = imgs_[img_idx];

                if (std::abs(compose_scale - 1) > 1e-1)
                {
    #if ENABLE_LOG
                    int64 resize_t = getTickCount();
    #endif
                    resize(full_img, img, Size(), compose_scale, compose_scale);
                }
                else
                    img = full_img;
                full_img.release();
                Size img_size = img.size();

                Mat K;
                cameras_[img_idx].K().convertTo(K, CV_32F);

    #if ENABLE_LOG
                int64 pt = getTickCount();
    #endif

    #ifdef SPEEDRUN
                t = cv::getTickCount();
    #endif

                //save map file
                if (is_first)
                {
                    dst_roi[img_idx] = w[img_idx]->buildMaps(img.size(), K, cameras_[img_idx].R, uxmap[img_idx], uymap[img_idx]);
//                    if (isCuda)
//                    {
//                        d_xmap[img_idx].upload(uxmap[img_idx]);
//                        d_ymap[img_idx].upload(uymap[img_idx]);
//                    }
    #ifdef SPEEDRUN
                    std::cout << "First build maps additional time: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec" << std::endl;
    #endif
                }
                img_warped.create(dst_roi[img_idx].height + 1, dst_roi[img_idx].width + 1, img.type());
//                if (isCuda)
//                {
//                    d_src.upload(img);
//                    cuda::remap(d_src, d_dst, d_xmap[img_idx], d_ymap[img_idx], INTER_LINEAR, BORDER_CONSTANT);
//                    d_dst.download(img_warped);
//                }
//                else
                    remap(img, img_warped, uxmap[img_idx], uymap[img_idx], INTER_LINEAR, BORDER_CONSTANT);

    #if ENABLE_LOG
                pt = getTickCount();
    #endif


                //save map file
                if (is_first)
                {
                    // Warp the current image mask
                    mask.create(img_size, CV_8U);
                    mask.setTo(Scalar::all(255));
                    w[img_idx]->warp(mask, K, cameras_[img_idx].R, INTER_NEAREST, BORDER_CONSTANT, mask_warped[img_idx]);
                }

    #ifdef SPEEDRUN
                std::cout << "Warp time: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec" << std::endl;
    #endif

    #if ENABLE_LOG
                pt = getTickCount();
    #endif

                // Compensate exposure
                exposure_comp_->apply((int)img_idx, corners[img_idx], img_warped, mask_warped[img_idx]);

    #if ENABLE_LOG
                pt = getTickCount();
    #endif
                img_warped.convertTo(img_warped_s[img_idx], CV_16S);
                img_warped.release();
                img.release();
                mask.release();

                // Make sure seam mask has proper size
                dilate(masks_warped[img_idx], dilated_mask, Mat());
                resize(dilated_mask, seam_mask, mask_warped[img_idx].size());

                bitwise_and(seam_mask, mask_warped[img_idx], mask_warped[img_idx]);

    #if ENABLE_LOG
                pt = getTickCount();
    #endif



    #if ENABLE_LOG
                int64 feed_t = getTickCount();
    #endif
                // Blend the current image

            }
#ifdef SPEEDRUN
        t = cv::getTickCount();
#endif
            for (int64 img_idx = 0; img_idx < imgs_.size(); ++img_idx)
            {
                blender_->feed(img_warped_s[img_idx], mask_warped[img_idx], corners[img_idx]);
            }
#ifdef SPEEDRUN
        std::cout << "Feed time: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec" << std::endl;
#endif
    #if ENABLE_LOG
            int64 blend_t = getTickCount();
    #endif
            UMat result, result_mask;

    #ifdef SPEEDRUN
            t = cv::getTickCount();
    #endif

            blender_->blend(result, result_mask);

    #ifdef SPEEDRUN
            std::cout << "Blend time: " << ((cv::getTickCount() - t) / cv::getTickFrequency()) << " sec" << std::endl;
    #endif

            //LOGLN("blend time: " << ((getTickCount() - blend_t) / getTickFrequency()) << " sec");

            //LOGLN("Compositing, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

            // Preliminary result is in CV_16SC3 format, but all values are in [0,255] range,
            // so convert it to avoid user confusing
            result.convertTo(pano, CV_8U);
            is_first = false;
            return OK;
}


Stitcherfix::Status Stitcherfix::stitch(InputArrayOfArrays images, OutputArray pano)
{
    Status status = estimateTransform(images);
    if (status != OK)
        return status;
    return composePanorama(pano);
}


Stitcherfix::Status Stitcherfix::stitch(InputArrayOfArrays images, const std::vector<std::vector<Rect> > &rois, OutputArray pano)
{
    Status status = estimateTransform(images, rois);
    if (status != OK)
        return status;
    return composePanorama(pano);
}

std::vector<detail::CameraParams> Stitcherfix::cameras() const { return cameras_g; }


Stitcherfix::Status Stitcherfix::matchImages()
{
    if ((int)imgs_.size() < 2)
    {
        LOGLN("Need more images");
        std::cout << "Need more images" << std::endl;
        return ERR_NEED_MORE_IMGS;
    }

    work_scale_ = 1;
    seam_work_aspect_ = 1;
    seam_scale_ = 1;
    bool is_work_scale_set = false;
    bool is_seam_scale_set = false;
    UMat full_img, img;
    features_.resize(imgs_.size());
    seam_est_imgs_.resize(imgs_.size());
    full_img_sizes_.resize(imgs_.size());

    LOGLN("Finding features...");
#if ENABLE_LOG
    int64 t = getTickCount();
#endif

    for (size_t i = 0; i < imgs_.size(); ++i)
    {
        full_img = imgs_[i];
        full_img_sizes_[i] = full_img.size();

        if (registr_resol_ < 0)
        {
            img = full_img;
            work_scale_ = 1;
            is_work_scale_set = true;
        }
        else
        {
            if (!is_work_scale_set)
            {
                work_scale_ = std::min(1.0, std::sqrt(registr_resol_ * 1e6 / full_img.size().area()));
                is_work_scale_set = true;
            }
            resize(full_img, img, Size(), work_scale_, work_scale_);
        }
        if (!is_seam_scale_set)
        {
            seam_scale_ = std::min(1.0, std::sqrt(seam_est_resol_ * 1e6 / full_img.size().area()));
            seam_work_aspect_ = seam_scale_ / work_scale_;
            is_seam_scale_set = true;
        }

        if (rois_.empty())
            (*features_finder_)(img, features_[i]);
        else
        {
            std::vector<Rect> rois(rois_[i].size());
            for (size_t j = 0; j < rois_[i].size(); ++j)
            {
                Point tl(cvRound(rois_[i][j].x * work_scale_), cvRound(rois_[i][j].y * work_scale_));
                Point br(cvRound(rois_[i][j].br().x * work_scale_), cvRound(rois_[i][j].br().y * work_scale_));
                rois[j] = Rect(tl, br);
            }
            (*features_finder_)(img, features_[i], rois);
        }
        features_[i].img_idx = (int)i;
        LOGLN("Features in image #" << i+1 << ": " << features_[i].keypoints.size());

        resize(full_img, img, Size(), seam_scale_, seam_scale_);
        seam_est_imgs_[i] = img.clone();
    }

    // Do it to save memory
    features_finder_->collectGarbage();
    full_img.release();
    img.release();

    LOGLN("Finding features, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

    LOG("Pairwise matching");
#if ENABLE_LOG
    t = getTickCount();
#endif
    (*features_matcher_)(features_, pairwise_matches_, matching_mask_);
    features_matcher_->collectGarbage();
    LOGLN("Pairwise matching, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

    // Leave only images we are sure are from the same panorama
    indices_ = detail::leaveBiggestComponent(features_, pairwise_matches_, (float)conf_thresh_);
    std::vector<UMat> seam_est_imgs_subset;
    std::vector<UMat> imgs_subset;
    std::vector<Size> full_img_sizes_subset;
    for (size_t i = 0; i < indices_.size(); ++i)
    {
        imgs_subset.push_back(imgs_[indices_[i]]);
        seam_est_imgs_subset.push_back(seam_est_imgs_[indices_[i]]);
        full_img_sizes_subset.push_back(full_img_sizes_[indices_[i]]);
    }
    if(imgs_subset.size() != imgs_.size())
    {
        std::cout << "Fail to pass thresh" << std::endl;
        return ERR_THRESH_FAIL;
    }
    seam_est_imgs_ = seam_est_imgs_subset;
    imgs_ = imgs_subset;
    full_img_sizes_ = full_img_sizes_subset;

    if ((int)imgs_.size() < 2)
    {
        LOGLN("Need more images");
        std::cout << "Fail to match" << std::endl;
        return ERR_NEED_MORE_IMGS;
    }

    return OK;
}


Stitcherfix::Status Stitcherfix::estimateCameraParams()
{
    detail::HomographyBasedEstimator estimator;
    if (!estimator(features_, pairwise_matches_, cameras_g))
        return ERR_HOMOGRAPHY_EST_FAIL;

    for (size_t i = 0; i < cameras_g.size(); ++i)
    {
        Mat R;
        cameras_g[i].R.convertTo(R, CV_32F);
        cameras_g[i].R = R;
        //LOGLN("Initial intrinsic parameters #" << indices_[i] + 1 << ":\n " << cameras_[i].K());
    }

    bundle_adjuster_->setConfThresh(conf_thresh_);
    if (!(*bundle_adjuster_)(features_, pairwise_matches_, cameras_g))
        return ERR_CAMERA_PARAMS_ADJUST_FAIL;

    // Find median focal length and use it as final image scale
    std::vector<double> focals;
    for (size_t i = 0; i < cameras_g.size(); ++i)
    {
        //LOGLN("Camera #" << indices_[i] + 1 << ":\n" << cameras_[i].K());
        focals.push_back(cameras_g[i].focal);
    }

    std::sort(focals.begin(), focals.end());
    if (focals.size() % 2 == 1)
        warped_image_scale_g = static_cast<float>(focals[focals.size() / 2]);
    else
        warped_image_scale_g = static_cast<float>(focals[focals.size() / 2 - 1] + focals[focals.size() / 2]) * 0.5f;

    if (do_wave_correct_)
    {
        std::vector<Mat> rmats;
        for (size_t i = 0; i < cameras_g.size(); ++i)
            rmats.push_back(cameras_g[i].R.clone());
        detail::waveCorrect(rmats, wave_correct_kind_);
        for (size_t i = 0; i < cameras_g.size(); ++i)
            cameras_g[i].R = rmats[i];
    }

    return OK;
}


Ptr<Stitcherfix> createStitcher(bool try_use_gpu)
{
    Ptr<Stitcherfix> stitcher = makePtr<Stitcherfix>();
    stitcher->setRegistrationResol(0.6);
    stitcher->setSeamEstimationResol(0.05);
    stitcher->setCompositingResol(Stitcherfix::ORIG_RESOL);
    stitcher->setPanoConfidenceThresh(1);
    stitcher->setWaveCorrection(true);
    stitcher->setWaveCorrectKind(detail::WAVE_CORRECT_HORIZ);
    stitcher->setFeaturesMatcher(makePtr<detail::BestOf2NearestMatcher>(try_use_gpu));
    stitcher->setBundleAdjuster(makePtr<detail::BundleAdjusterRay>());

    #ifdef HAVE_CUDA
    if (try_use_gpu && cuda::getCudaEnabledDeviceCount() > 0)
    {
        #ifdef HAVE_OPENCV_NONFREE
        stitcher->setFeaturesFinder(makePtr<detail::SurfFeaturesFinderGpu>());
        #else
        stitcher->setFeaturesFinder(makePtr<detail::OrbFeaturesFinder>());
        #endif
        stitcher->setWarper(makePtr<SphericalWarperGpu>());
        stitcher->setSeamFinder(makePtr<detail::GraphCutSeamFinderGpu>());
    }
    else
        #endif
        {
            #ifdef HAVE_OPENCV_NONFREE
            stitcher->setFeaturesFinder(makePtr<detail::SurfFeaturesFinder>());
            #else
            stitcher->setFeaturesFinder(makePtr<detail::OrbFeaturesFinder>());
            #endif
            stitcher->setWarper(makePtr<SphericalWarper>());
            stitcher->setSeamFinder(makePtr<detail::DpSeamFinder>());
            //stitcher->setSeamFinder(makePtr<detail::GraphCutSeamFinder>(detail::GraphCutSeamFinderBase::COST_COLOR));
        }

        stitcher->setExposureCompensator(makePtr<detail::NoExposureCompensator>());
        stitcher->setBlender(makePtr<detail::MultiBandBlender>(try_use_gpu));

        return stitcher;
}
} // namespace cv
