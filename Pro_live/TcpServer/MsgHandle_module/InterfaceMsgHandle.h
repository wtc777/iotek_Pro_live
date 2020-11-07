#ifndef INTERFACEMSGHANDLE_H
#define INTERFACEMSGHANDLE_H
#include "SocketHandle.h"
#include "Protocol.h"
#include "RoomHelper.h"
#include "OpeDB.h"
#include "NetworkHandle.h"
#include "UdpPortHelper.h"
//接收数据解析处理接口--简单工厂模式
class InterfaceMsgHandle
{
public:
    InterfaceMsgHandle();
    virtual void msgHandle(SocketHandle* ,PDU&);
    virtual ~InterfaceMsgHandle(){}
};

#endif // INTERFACEMSGHANDLE_H
