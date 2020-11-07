#ifndef ROOMACCESSHANDLE_H
#define ROOMACCESSHANDLE_H
#include "InterfaceMsgHandle.h"

class RoomAccessHandle:public InterfaceMsgHandle
{
public:
    RoomAccessHandle();
    void msgHandle(PDU&pdu);
};

#endif // ROOMACCESSHANDLE_H
