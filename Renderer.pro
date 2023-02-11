QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    color/color.cpp \
    controller/controller.cpp \
    interface/mainwindow.cpp \
    interface/renderwidget.cpp \
    main.cpp \
    scene/light/light.cpp \
    scene/model/model.cpp \
    scene/camera/camera.cpp \
    scene/camera/cameramanager.cpp \
    scene/model/basemodel.cpp \
    scene/model/modelmanager.cpp \
    scene/render/lightshader.cpp \
    scene/render/rendermanager.cpp \
    scene/render/sceneshader.cpp \
    scene/render/shadowmodel.cpp

HEADERS += \
    color/color.h \
    controller/controller.h \
    geometry/geometry.h \
    geometry/geometry.hpp \
    geometry/mathvector.h \
    geometry/mathvector.hpp \
    geometry/matrix.h \
    geometry/matrix.hpp \
    interface/mainwindow.h \ \
    interface/renderwidget.h \
    scene/light/light.h \
    scene/model/model.h \
    scene/camera/camera.h \
    scene/camera/cameramanager.h \
    scene/model/basemodel.h \
    scene/model/modelmanager.h \
    scene/render/baseshader.h \
    scene/render/lightshader.h \
    scene/render/rendermanager.h \
    scene/render/sceneshader.h \
    scene/render/shadowmodel.h \
    scene/scene.h

FORMS += \
    interface/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
