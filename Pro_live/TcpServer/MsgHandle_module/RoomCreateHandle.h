#ifndef ROOMCREATEHANDLE_H
#define ROOMCREATEHANDLE_H

#include "InterfaceMsgHandle.h"

class RoomCreateHandle:public InterfaceMsgHandle
{
public:
    RoomCreateHandle();
    virtual void msgHandle(SocketHandle* handle, PDU &pdu);
};

#endif // ROOMCREATEHANDLE_H
