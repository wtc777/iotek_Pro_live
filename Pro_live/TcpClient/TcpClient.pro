QT       += core gui
QT      +=network
QT      +=multimedia
QT      +=multimediawidgets
RC_ICONS = foot.ico
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Login.cpp \
    Protocol.cpp \
    main.cpp \
    RoomOpeWidget.cpp \
    LiveOpeWidget.cpp \
    MyThread/LabelMoveThread.cpp \
    VideoSurface.cpp \
    MsgHandle_module/InterfaceMsgHandle.cpp \
    ReceMsgHandle.cpp \
    MsgHandle_module/RoomCreateHandle.cpp \
    MsgHandle_module/RoomAccessHandle.cpp \
    Log_module/myLog.cpp \
    IPconfig.cpp \
    RegistWidget.cpp

HEADERS += \
    Login.h \
    Protocol.h \
    RoomOpeWidget.h \
    LiveOpeWidget.h \
    MyThread/LabelMoveThread.h \
    VideoSurface.h \
    MsgHandle_module/InterfaceMsgHandle.h \
    ReceMsgHandle.h \
    MsgHandle_module/RoomCreateHandle.h \
    MsgHandle_module/RoomAccessHandle.h \
    Log_module/myLog.h \
    IPconfig.h \
    RegistWidget.h

FORMS += \
    Login.ui \
    RoomOpeWidget.ui \
    LiveOpeWidget.ui \
    Log_module/myLog.ui \
    IPconfig.ui \
    RegistWidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
