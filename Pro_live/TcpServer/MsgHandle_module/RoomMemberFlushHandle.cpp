#include "RoomMemberFlushHandle.h"

RoomMemberFlushHandle::RoomMemberFlushHandle()
{

}

void RoomMemberFlushHandle::msgHandle(SocketHandle *socket, PDU &package)
{
    QVector<Account>memberList = RoomHelper::getInstance()->handlegetRoomMemberlist(package);
    QVector<Account>::iterator it = memberList.begin();
    int i = 0;
    PDU respackage(PDU::MSG_TYPE_FLUSH_ROOM_MEMBERLIST_RESPOND);
    for(; it != memberList.end(); it++, i++){
        QString key = JSON_KEY_MEMBERNAME + QString::number(i);
        respackage[key] = it->getName();
        qDebug()<<it->getName();
    }
    socket->getClientsocket()->write(respackage.pack());
}
