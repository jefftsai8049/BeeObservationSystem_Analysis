#-------------------------------------------------
#
# Project created by QtCreator 2015-05-21T12:33:31
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BeeObservationSystem_Analysis
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    trajectory_tracking.cpp \
    tag_recognition.cpp \
    qsmartgraphicsview/qsmartgraphicsview.cpp \
    qsmartgraphicsview/qsmartlabel.cpp

HEADERS  += mainwindow.h \
    trajectory_tracking.h \
    tag_recognition.h \
    qsmartgraphicsview/qsmartgraphicsview.h \
    qsmartgraphicsview/qsmartlabel.h

FORMS    += mainwindow.ui

msvc {
  QMAKE_CXXFLAGS += -openmp -arch:AVX -D "_CRT_SECURE_NO_WARNINGS"
  QMAKE_CXXFLAGS_RELEASE *= -O2
}

INCLUDEPATH += C:\\opencv300_x64_vc2013\\include \
                C:\\opencv300_x64_vc2013\\include\\opencv \
                C:\\opencv300_x64_vc2013\\include\\opencv2 \

##debug libs

LIBS +=  C:\\opencv300_x64_vc2013\\lib\\opencv_world300.lib \
                C:\\opencv300_x64_vc2013\\lib\\opencv_ts300.lib \
#release libs
                C:\\opencv300_x64_vc2013\\lib\\opencv_world300d.lib \
                C:\\opencv300_x64_vc2013\\lib\\opencv_ts300d.lib

#LIBS += C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_calib3d300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_ccalib300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_core300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_cudaarithm300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_cudabgsegm300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_cudacodec300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_cudafeatures2d300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_cudafilters300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_cudaimgproc300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_cudalegacy300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_cudaobjdetect300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_cudaoptflow300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_cudastereo300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_cudawarping300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_cudev300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_datasets300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_face300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_features2d300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_ffmpeg300_64.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_flann300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_highgui300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_imgcodecs300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_imgproc300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_latentsvm300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_line_descriptor300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_ml300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_objdetect300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_optflow300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_photo300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_reg300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_rgbd300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_saliency300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_stitching300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_shape300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_stitching300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_superres300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_surface_matching300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_text300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_tracking300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_ts300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_video300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_videoio300.lib \
#        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_videostab300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_xfeatures2d300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_ximgproc300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_xobjdetect300.lib \
##        C:\\opencv300_x64_vc2013_cuda\\lib\\opencv_xphoto300.lib


DEFINES += HAVE_OPENCV \
#        += DEBUG_TAG_RECOGNITION

RESOURCES += \
    icon.qrc
