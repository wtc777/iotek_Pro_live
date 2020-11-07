#ifndef REGISTHANDLE_H
#define REGISTHANDLE_H
#include "InterfaceMsgHandle.h"

class RegistHandle : public InterfaceMsgHandle
{
public:
    RegistHandle();
    virtual void msgHandle(SocketHandle* handle, PDU&pdu);
};

#endif // REGISTHANDLE_H
