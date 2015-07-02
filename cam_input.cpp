#include "cam_input.h"

cam_input::cam_input(QObject *parent) :
    QThread(parent)
{
    cv::ocl::setUseOpenCL(true);
}


void cam_input::run()
{
    if(!readyForComposite)
        return;

    std::vector<cv::VideoCapture> cap(camera_num);
    if(mode == 2)
    {
        for(int i = 0; i < camera_num; i++)
        {
            cap[i].open(video_names[i]);
            if(!cap[i].isOpened())
            {
                return;
            }
        }
    }
    else if(mode == 1)
    {
        for(int i = 0; i < camera_num; i++)
        {
            cap[i].open(i);
            if(!cap[i].isOpened())
            {
                return;
            }
        }
    }
    else
    {
        return;
    }




    stopped = false;
    cv::VideoCapture c;


    cv::Mat pano;
    time_t cl;
    while(!stopped)
    {
        std::vector<cv::Mat> frame(camera_num);
        for(int i = 0; i < camera_num; i++)
        {
            cap[i].read(frame[i]);
        }

        cv::waitKey(10);
        cl = clock();
        sfx->composePanorama(frame, pano);
        std::cout << clock() - cl << std::endl << std::endl;


        cv::imshow("stitch", pano);



        //sendPano(frame[1]);
    }

}

int cam_input::initialCam(int cam_num)
{
    camera_num = cam_num;
    cv::Mat pano;
    sfx = new cv::Stitcherfix(cv::Stitcherfix::createDefault(true));

    std::vector<cv::VideoCapture> cap(camera_num);
    std::vector<cv::Mat> frame(camera_num);

    for(int i = 0; i < camera_num; i++)
    {
        cap[i].open(i);
    }

    for(int i = 0; i < camera_num; i++)
    {
        cap[i] >> frame[i];
    }

    //Stitching Zone
    cv::Stitcherfix::Status status = sfx->estimateTransform(frame);


    if (status != cv::Stitcherfix::OK)
    {
        std::cout << "Can't stitch images, error code = " << int(status) << std::endl;

        return status;
    }

    cameras = sfx->cameras();
    img_sz = frame[0].size();
    readyForComposite = true;

//    saveKR();
    saveSetting();
    sfx->composePanorama(frame, pano);
    cv::imwrite("KRResult.png", pano);
    //Stitching End

    mode = 1;

    return status;
}

int cam_input::initialVideo(std::vector<std::string> filenames)
{
    camera_num = filenames.size();
    cv::Mat pano;
    sfx = new cv::Stitcherfix(cv::Stitcherfix::createDefault(true));

    //std::vector<cv::VideoCapture> cap(camera_num);
    std::vector<cv::Mat> frame(camera_num);

//    for(int i = 0; i < camera_num; i++)
//    {
//        cap[i].open(video_names[i]);
//    }

    for(int i = 0; i < camera_num; i++)
    {
        frame[i] = cv::imread(filenames[i]);
    }

    //Stitching Zone
    sfx->setPanoConfidenceThresh(0.5);
    sfx->setWarper(cv::makePtr<cv::PlaneWarper>());
    cv::Stitcherfix::Status status = sfx->estimateTransform(frame);


    if (status != cv::Stitcherfix::OK)
    {
        std::cout << "Can't stitch images, error code = " << int(status) << std::endl;

        return status;
    }


    cameras = sfx->cameras();
    img_sz = frame[0].size();
    readyForComposite = true;

//    saveKR();
    saveSetting();
    sfx->composePanorama(frame, pano);
    cv::imwrite("KRResult.png", pano);
    //Stitching End

    mode = 2;
    sendInitial(camera_num, frame, pano);

    return status;
}



void cam_input::saveKR()
{
    std::ofstream outFile("KR.txt");
    outFile << cameras.size() << std::endl;
    outFile << img_sz.width << " " << img_sz.height << std::endl;
    for (size_t i = 0; i < cameras.size(); ++i){
        // focal length, aspect, ppx, ppy, R
        outFile << i + 1 << " ";
        outFile << cameras[i].focal << " ";     //focal
        outFile << cameras[i].aspect << " ";    //Aspect ratio
        outFile << cameras[i].ppx << " ";       //Principal point X
        outFile << cameras[i].ppy << " ";       //Principal point Y
        for (size_t j = 0; j < 9; ++j)
            outFile << cameras[i].R.ptr<float>(j / 3)[j % 3] << " ";//Rotation
        outFile << std::endl;
    }
    outFile.close();
}

void cam_input::saveSetting()
{
    std::string fileName = "stitching.xml";
    cv::FileStorage f(fileName,cv::FileStorage::WRITE);
    int size = cameras.size();
    f << "cameras_size" << size;
    f << "img_sz_width" << img_sz.width;
    f << "img_sz_height" << img_sz.height;
    for (int i = 0; i < cameras.size(); ++i)
    {
        // focal length, aspect, ppx, ppy, R
        f << "cameras_focal_"+std::to_string(i) << cameras[i].focal;     //focal
        f << "cameras_aspect_"+std::to_string(i) << cameras[i].aspect;    //Aspect ratio
        f << "cameras_ppx_"+std::to_string(i) << cameras[i].ppx;       //Principal point X
        f << "cameras_ppy_"+std::to_string(i) << cameras[i].ppy;       //Principal point Y
        f << "cameras_R_"+std::to_string(i) << cameras[i].R;       //Rotation
    }
    f.release();
    std::cout << "Stitching Setting Saved!\n";
}

//void cam_input::loadSetting(const std::string &fileName)
//{
//    cv::FileStorage f(fileName,cv::FileStorage::READ);
////    int size = cameras.size();
//    f["cameras_size"] >> cameras.size();
//    f["img_sz_width"] >> img_sz.width;
//    f << "img_sz_height" << img_sz.height;
//    for (int i = 0; i < cameras.size(); ++i)
//    {
//        // focal length, aspect, ppx, ppy, R
//        f << "cameras_focal_"+std::to_string(i) << cameras[i].focal;     //focal
//        f << "cameras_aspect_"+std::to_string(i) << cameras[i].aspect;    //Aspect ratio
//        f << "cameras_ppx_"+std::to_string(i) << cameras[i].ppx;       //Principal point X
//        f << "cameras_ppy_"+std::to_string(i) << cameras[i].ppy;       //Principal point Y
//        f << "cameras_R_"+std::to_string(i) << cameras[i].R;       //Rotation
//    }
//    f.release();
//    qDebug() << "Stitching Setting Saved!\n";
//}

