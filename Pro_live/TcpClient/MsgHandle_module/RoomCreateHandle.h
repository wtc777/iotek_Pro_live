#ifndef ROOMCREATEHANDLE_H
#define ROOMCREATEHANDLE_H

#include "InterfaceMsgHandle.h"
class RoomCreateHandle : public InterfaceMsgHandle
{
public:
    RoomCreateHandle();
    void msgHandle(PDU&pdu);
};

#endif // ROOMCREATEHANDLE_H
