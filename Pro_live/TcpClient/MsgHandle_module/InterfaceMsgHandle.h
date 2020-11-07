#ifndef INTERFACEMSGHANDLE_H
#define INTERFACEMSGHANDLE_H
#include "Protocol.h"
#include "LiveOpeWidget.h"
#include "RoomOpeWidget.h"
#include "Login.h"
#include <QMessageBox>
//接收数据解析处理接口--简单工厂模式
class InterfaceMsgHandle
{
public:
    InterfaceMsgHandle();
    virtual void msgHandle(PDU&);
    virtual ~InterfaceMsgHandle(){}
};

#endif // INTERFACEMSGHANDLE_H
