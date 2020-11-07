#ifndef RECEMSGHANDLE_H
#define RECEMSGHANDLE_H

#include <QTcpSocket>
#include "Protocol.h"
#include "MsgHandle_module/InterfaceMsgHandle.h"

class ReceMsgHandle
{
public:
    ReceMsgHandle();
    void MsgHandle(SocketHandle *handle, PDU& pdu);
};

#endif // RECEMSGHANDLE_H
