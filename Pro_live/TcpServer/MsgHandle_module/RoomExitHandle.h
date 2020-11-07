#ifndef ROOMEXITHANDLE_H
#define ROOMEXITHANDLE_H
#include "InterfaceMsgHandle.h"

class RoomExitHandle : public InterfaceMsgHandle
{
public:
    RoomExitHandle();

    virtual void msgHandle(SocketHandle* socket, PDU& package);
};

#endif // ROOMEXITHANDLE_H
