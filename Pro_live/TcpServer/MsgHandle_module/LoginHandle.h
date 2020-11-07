#ifndef LOGINHANDLE_H
#define LOGINHANDLE_H
#include "InterfaceMsgHandle.h"

class LoginHandle : public InterfaceMsgHandle
{
public:
    LoginHandle();
    virtual void msgHandle(SocketHandle* handle, PDU &pdu);
};

#endif // LOGINHANDLE_H
