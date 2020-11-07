#include "RoomAccessHandle.h"
RoomAccessHandle::RoomAccessHandle()
{

}

void RoomAccessHandle::msgHandle(SocketHandle *handle, PDU &pdu)
{    
    bool ret = RoomHelper::getInstance()->handleJoinRoom(handle,pdu);
    PDU respackage(PDU::MSG_TYPE_ACCESS_ROOM_RESPOND);
    respackage[JSON_KEY_ROOMNAME] = pdu[JSON_KEY_ROOMNAME];
    QString roomName = respackage[JSON_KEY_ROOMNAME].toString();
    if(ret){
        respackage[JSON_KEY_SERVER_RESPOND] = STR_ACCESSROOM_SUCCESS;
        Room* room = RoomHelper::getInstance()->handleGetRoom(roomName);
        respackage[JSON_KEY_GROUPADDRESS] = room->getgroupAddress().toString();
        respackage[JSON_KEY_GROUPPORT] = room->getgroupPort();
        respackage[JSON_KEY_GROUPVOICEPORT] = room->getgroupVoicePort();
    }else{
        respackage[JSON_KEY_SERVER_RESPOND] = STR_ACCESSROOM_REFUSED;
    }
    handle->getClientsocket()->write(respackage.pack());
}
