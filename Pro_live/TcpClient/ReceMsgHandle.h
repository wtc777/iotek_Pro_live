#ifndef RECEMSGHANDLE_H
#define RECEMSGHANDLE_H

#include "MsgHandle_module/InterfaceMsgHandle.h"
#include <QMessageBox>
class ReceMsgHandle
{
public:
    ReceMsgHandle();
    void msgHandle(PDU &pdu);
};

#endif // RECEMSGHANDLE_H
