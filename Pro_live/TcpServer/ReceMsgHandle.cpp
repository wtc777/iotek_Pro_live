#include "ReceMsgHandle.h"
#include "MsgHandle_module/RegistHandle.h"
#include "MsgHandle_module/LoginHandle.h"
#include "MsgHandle_module/RoomCreateHandle.h"
#include "MsgHandle_module/RoomlistFlushHandle.h"
#include "MsgHandle_module/RoomAccessHandle.h"
#include "MsgHandle_module/RoomMemberFlushHandle.h"
#include "MsgHandle_module/RoomChatHandle.h"
#include "MsgHandle_module/RoomExitHandle.h"
#include "MsgHandle_module/RoomGiftHandle.h"
ReceMsgHandle::ReceMsgHandle()
{

}

void ReceMsgHandle::MsgHandle(SocketHandle *handle, PDU &pdu)
{
    InterfaceMsgHandle* in = nullptr;
    switch (pdu.getPDUtype()) {
    case PDU::MSG_TYPE_REGIST_REQUEST:{
        in = new RegistHandle;
        in->msgHandle(handle,pdu);
        break;
    }
    case PDU::MSG_TYPE_LOGIN_REQUEST:{
        in = new LoginHandle;
        in->msgHandle(handle,pdu);
        break;
    }
    case PDU::MSG_TYPE_ROOM_CREATE_REQUEST:{
        in = new RoomCreateHandle;
        in->msgHandle(handle,pdu);
        break;
    }
    case PDU::MSG_TYPE_ROOMLIST_FLUSH_REQUEST:{
        in = new RoomlistFlushHandle;
        in->msgHandle(handle,pdu);
        break;
    }
    case PDU::MSG_TYPE_ACCESS_ROOM_REQUEST:{
        in = new RoomAccessHandle;
        in->msgHandle(handle,pdu);
        break;
    }
    case PDU::MSG_TYPE_FLUSH_ROOM_MEMBERLIST_REQUEST:{
        in = new RoomMemberFlushHandle;
        in->msgHandle(handle,pdu);
        break;
    }
    case PDU::MSG_TYPE_ROOM_CHAT_REQUEST:{
        in = new RoomChatHandle;
        in->msgHandle(handle,pdu);
        break;
    }
    case PDU::MSG_TYPE_EXIT_ROOM_REQUEST:{
        in = new RoomExitHandle;
        in->msgHandle(handle,pdu);
        break;
    }
    case PDU::MSG_TYPE_SEND_GIFT_REQUEST:{
        in = new RoomGiftHandle;
        in->msgHandle(handle,pdu);
        qDebug()<<"gift";
        break;
    }
    default:
        break;
    }
    delete in;
    in = nullptr;
}
