#ifndef CAM_INPUT_H
#define CAM_INPUT_H

#include <QThread>
#include <QFileDialog>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include "time.h"
#include "stitcherfix/stitcherfix.h"

class cam_input : public QThread
{
    Q_OBJECT
public:
    explicit cam_input(QObject *parent = 0);
    void stopCam(bool status){stopped = status;}
    int initialCam(int cam_num);
    int initialVideo(std::vector<std::string> filenames);
    void setVideoName(std::vector<std::string> filenames){video_names = filenames;}

    void loadSetting(const std::string &fileName);

protected:
    void run();
private:
    bool stopped = false;
    cv::Stitcherfix *sfx;
    std::vector<cv::detail::CameraParams> cameras;
    cv::Size img_sz;
    bool readyForComposite = false;

    void saveKR();
    void saveSetting();
    int camera_num = 0;
    std::vector<std::string> video_names;
    int mode = 0; // 0 for nothing, 1 for camera, 2 for video
signals:
    void sendPano(cv::Mat pano);
    void sendInitial(int numl, std::vector<cv::Mat> image_set, cv::Mat pano);
public slots:

};

#endif // CAM_INPUT_H
