#ifndef ROOMCHATHANDLE_H
#define ROOMCHATHANDLE_H

#include "InterfaceMsgHandle.h"

class RoomChatHandle : public InterfaceMsgHandle
{
public:
    RoomChatHandle();
    virtual void msgHandle(SocketHandle* handle, PDU &pdu);
};

#endif // ROOMCHATHANDLE_H
