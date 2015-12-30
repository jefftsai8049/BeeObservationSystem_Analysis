#-------------------------------------------------
#
# Project created by QtCreator 2015-05-21T12:33:31
#
#-------------------------------------------------

QT       += core gui opengl serialport concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BeeObservationSystem_Analysis
TEMPLATE = app

DEFINES += HAVE_OPENCV \
#        += DEBUG_TAG_RECOGNITION \
#        += SAVE_TAG_IMAGE \
#        += DEBUG_OBJECT_TRACKING \
        += NO_OCL \
#        += SHOW_PATTERN_NAME

CONFIG += HAVE_OPENCV \
#        += HAVE_TESSERACT \

SOURCES += main.cpp\
        mainwindow.cpp \
    trajectory_tracking.cpp \
    tag_recognition.cpp \
    qsmartgraphicsview/qsmartgraphicsview.cpp \
    qsmartgraphicsview/qsmartlabel.cpp \
    object_tracking.cpp \
    math_function.cpp \
    dataprocesswindow.cpp \
    objecttrackingform.cpp

HEADERS  += mainwindow.h \
    trajectory_tracking.h \
    tag_recognition.h \
    qsmartgraphicsview/qsmartgraphicsview.h \
    qsmartgraphicsview/qsmartlabel.h \
    object_tracking.h \
    math_function.h \
    dataprocesswindow.h \
    objecttrackingform.h

FORMS    += mainwindow.ui \
    dataprocesswindow.ui \
    objecttrackingform.ui

msvc {
  QMAKE_CXXFLAGS += -openmp -arch:AVX -D "_CRT_SECURE_NO_WARNINGS"
  QMAKE_CXXFLAGS_RELEASE *= -O2
}


HAVE_OPENCV{
    INCLUDEPATH += C:\\opencv300_vc2013\\include \
                    C:\\opencv300_vc2013\\include\\opencv \
                    C:\\opencv300_vc2013\\include\\opencv2 \

    LIBS += -LC:\\opencv300_vc2013\\x64\\lib -lopencv_world300d -lopencv_world300 -lopencv_ts300d -lopencv_ts300

}


#HAVE_TESSERACT{
#    LIBS += -LC:\\tesseract\\lib -llibtesseract302 -llibtesseract302d -llibtesseract302-static -llibtesseract302-static-debug -lliblept168 -lliblept168d
#}

RESOURCES += \
    icon.qrc
