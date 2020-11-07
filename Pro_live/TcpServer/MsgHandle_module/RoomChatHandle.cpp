#include "RoomChatHandle.h"

RoomChatHandle::RoomChatHandle()
{

}

void RoomChatHandle::msgHandle(SocketHandle *handle, PDU &pdu)
{
    //计算积分并返回
    int size = pdu[JSON_KEY_CHATMSG].toString().size();
    QString name = pdu[JSON_KEY_NAME].toString();
    int score = OpeDB::getInstance()->handleGetScore(name.toStdString().c_str()) + size;
    OpeDB::getInstance()->handleupdateScore(name.toStdString().c_str(),score);
    PDU respackage(PDU::MSG_TYPE_ROOM_CHAT_RESPOND);
    respackage[JSON_KEY_SCORE] = score;
    handle->getClientsocket()->write(respackage.pack());
    //获取聊天室成员列表
    QVector<Account> members = RoomHelper::getInstance()->handlegetRoomMemberlist(pdu);
    //转发消息给房主
    NetworkHandle::getinstance()->reSend(pdu[JSON_KEY_ROOMNAME].toString(),pdu);
    QVector<Account>::iterator it = members.begin();
    for(; it != members.end(); it++){
        NetworkHandle::getinstance()->reSend(it->getName(),pdu);//转发数据包到所有数据端
    }
}
