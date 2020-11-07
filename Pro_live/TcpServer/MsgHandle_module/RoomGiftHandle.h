#ifndef ROOMGIFTHANDLE_H
#define ROOMGIFTHANDLE_H
#include "InterfaceMsgHandle.h"

class RoomGiftHandle : public InterfaceMsgHandle
{
public:
    RoomGiftHandle();
    virtual void msgHandle(SocketHandle* handle,PDU& pdu);
};

#endif // ROOMGIFTHANDLE_H
