QT       += core gui
QT      +=network
QT      +=sql
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
    DBHelper.cpp \
    NetworkHandle.cpp \
    OpeDB.cpp \
    Protocol.cpp \
    SocketHandle.cpp \
    main.cpp \
    MainWindow.cpp \
    ReceMsgHandle.cpp \
    Room.cpp \
    RoomHelper.cpp \
    Log_module/myLog.cpp \
    MsgHandle_module/LoginHandle.cpp \
    MsgHandle_module/RegistHandle.cpp \
    MsgHandle_module/RoomCreateHandle.cpp \
    MsgHandle_module/InterfaceMsgHandle.cpp \
    MsgHandle_module/RoomlistFlushHandle.cpp \
    MsgHandle_module/RoomAccessHandle.cpp \
    MsgHandle_module/RoomChatHandle.cpp \
    MsgHandle_module/RoomMemberFlushHandle.cpp \
    MsgHandle_module/RoomExitHandle.cpp \
    UdpPortHelper.cpp \
    MsgHandle_module/RoomGiftHandle.cpp

HEADERS += \
    DBHelper.h \
    MainWindow.h \
    NetworkHandle.h \
    OpeDB.h \
    Protocol.h \
    SocketHandle.h \
    ReceMsgHandle.h \
    Room.h \
    RoomHelper.h \
    Log_module/myLog.h \
    MsgHandle_module/LoginHandle.h \
    MsgHandle_module/RegistHandle.h \
    MsgHandle_module/RoomCreateHandle.h \
    MsgHandle_module/InterfaceMsgHandle.h \
    MsgHandle_module/RoomlistFlushHandle.h \
    MsgHandle_module/RoomAccessHandle.h \
    MsgHandle_module/RoomChatHandle.h \
    MsgHandle_module/RoomMemberFlushHandle.h \
    MsgHandle_module/RoomExitHandle.h \
    UdpPortHelper.h \
    MsgHandle_module/RoomGiftHandle.h

FORMS += \
    MainWindow.ui \
    Log_module/myLog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
