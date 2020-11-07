#include "RoomCreateHandle.h"
RoomCreateHandle::RoomCreateHandle()
{

}

void RoomCreateHandle::msgHandle(SocketHandle *handle, PDU &pdu)
{    
    int ret = RoomHelper::getInstance()->handleCreateRoom(handle,pdu);
    PDU respackage(PDU::MSG_TYPE_ROOM_CREATE_RESPOND);
    respackage[JSON_KEY_ROOMNAME] = pdu[JSON_KEY_ROOMNAME];
    QString roomName = respackage[JSON_KEY_ROOMNAME].toString();        
    if(0 == ret || 1 == ret){
        if(0 == ret){
            respackage[JSON_KEY_SERVER_RESPOND] = STR_ROOMCREATE_SUCCESS;
            qDebug()<<"createroom IP:"<<respackage[JSON_KEY_GROUPADDRESS].toString()<<"port:"<<respackage[JSON_KEY_GROUPPORT].toInt();
        }else if(1 == ret){
            respackage[JSON_KEY_SERVER_RESPOND] = STR_ROOMCREATE_EXIST;
        }
        Room* room = RoomHelper::getInstance()->handleGetRoom(roomName);
        respackage[JSON_KEY_GROUPADDRESS] = room->getgroupAddress().toString();
        respackage[JSON_KEY_GROUPPORT] = room->getgroupPort();
        respackage[JSON_KEY_GROUPVOICEPORT] = room->getgroupVoicePort();
        respackage[JSON_KEY_SIGVIDEOPORT] = room->getsigVideoPort();
    }else{
        respackage[JSON_KEY_SERVER_RESPOND] = STR_UNKNOWN_ERROR;
    }

    handle->getClientsocket()->write(respackage.pack());
}
