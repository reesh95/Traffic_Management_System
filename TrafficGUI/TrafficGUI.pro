#-------------------------------------------------
#
# Project created by QtCreator 2017-04-06T22:48:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrafficGUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    player.cpp \
    Blob.cpp \
    traffic_light.cpp

HEADERS  += mainwindow.h \
    player.h \
    Blob.h \
    traffic_light.h \
    LCDNumber.h

FORMS    += mainwindow.ui

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/statemachine/trafficlight
INSTALLS += target

INCLUDEPATH += C:\MyFolder\Nitesh\Anveshan\opencv\building\install\include
LIBS += -LC:\MyFolder\Nitesh\Anveshan\opencv\building\install\x86\mingw\lib
LIBS += -llibopencv_calib3d320
LIBS += -llibopencv_core320
LIBS += -llibopencv_features2d320
LIBS += -llibopencv_flann320
LIBS += -llibopencv_highgui320
LIBS += -llibopencv_imgcodecs320
LIBS += -llibopencv_imgproc320
LIBS += -llibopencv_ml320
LIBS += -llibopencv_objdetect320
LIBS += -llibopencv_photo320
LIBS += -llibopencv_shape320
LIBS += -llibopencv_stitching320
LIBS += -llibopencv_superres320
LIBS += -llibopencv_video320
LIBS += -llibopencv_videoio320
LIBS += -llibopencv_videostab320
