#ifndef ROOMMEMBERFLUSHHANDLE_H
#define ROOMMEMBERFLUSHHANDLE_H

#include "InterfaceMsgHandle.h"

class RoomMemberFlushHandle : public InterfaceMsgHandle
{
public:
    RoomMemberFlushHandle();
    virtual void msgHandle(SocketHandle* socket, PDU& package);
};

#endif // ROOMMEMBERFLUSHHANDLE_H
