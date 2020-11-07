#ifndef ROOMACCESSHANDLE_H
#define ROOMACCESSHANDLE_H

#include "InterfaceMsgHandle.h"

class RoomAccessHandle : public InterfaceMsgHandle
{
public:
    RoomAccessHandle();
    virtual void msgHandle(SocketHandle* handle, PDU &pdu);
};

#endif // ROOMACCESSHANDLE_H
