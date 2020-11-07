#include "RoomlistFlushHandle.h"

RoomlistFlushHandle::RoomlistFlushHandle()
{

}

void RoomlistFlushHandle::msgHandle(SocketHandle *socket, PDU &package)
{
    QVector<QString>rooms = RoomHelper::getInstance()->handlegetRoomlist();

    PDU respackage(PDU::MSG_TYPE_ROOMLIST_FLUSH_RESPOND);
    QVector<QString>::iterator it = rooms.begin();
    int i = 1;
    for(; it != rooms.end(); it++, i++){
        QString key = JSON_KEY_ROOMLIST +QString::number(i);
        respackage[key] = *it;
    }
    socket->getClientsocket()->write(respackage.pack());
}
