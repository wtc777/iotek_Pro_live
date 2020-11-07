#ifndef ROOMLISTFLUSHHANDLE_H
#define ROOMLISTFLUSHHANDLE_H
#include "InterfaceMsgHandle.h"

class RoomlistFlushHandle : public InterfaceMsgHandle
{
public:
    RoomlistFlushHandle();
    virtual void msgHandle(SocketHandle* socket, PDU& package);
};

#endif // ROOMLISTFLUSHHANDLE_H
