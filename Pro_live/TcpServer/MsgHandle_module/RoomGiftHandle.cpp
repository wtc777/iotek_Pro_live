#include "RoomGiftHandle.h"
#include "Log_module/myLog.h"
RoomGiftHandle::RoomGiftHandle()
{

}

void RoomGiftHandle::msgHandle(SocketHandle *handle, PDU &pdu)
{
    OpeDB* helper = OpeDB::getInstance();
    qDebug()<<"gift handle";
    //扣除打赏者积分并返回余额
    PDU respackage1(PDU::MSG_TYPE_SEND_GIFT_RESPOND);
    int cost = pdu[JSON_KEY_SCORE].toInt();
    const char* name = pdu[JSON_KEY_NAME].toString().toStdString().c_str();
    int scoreafter = helper->handleGetScore(name) - cost;
    helper->handleupdateScore(name,scoreafter);
    QStringList msg1;
    msg1<<"打赏记录:"<<name<<QString("打赏:%1,余额:%2")
          .arg(QString::number(cost)).arg(scoreafter);
    MYLOG(msg1);
    respackage1[JSON_KEY_SCORE] = helper->handleGetScore(name);
    handle->getClientsocket()->write(respackage1.pack());
    //将积分累加给群主并返回余额
    PDU respackage2(PDU::MSG_TYPE_SEND_GIFT_RESPOND);
    const char* roomname = pdu[JSON_KEY_ROOMNAME].toString().toStdString().c_str();
    scoreafter = helper->handleGetScore(roomname) + cost;
    helper->handleupdateScore(roomname,scoreafter);
    msg1.clear();
    msg1<<"主播记录:"<<roomname<<QString("收入%1,余额:%2")
          .arg(QString::number(cost)).arg(QString::number(scoreafter));
    MYLOG(msg1);
    respackage2[JSON_KEY_SCORE] = helper->handleGetScore(roomname);
    NetworkHandle::getinstance()->reSend(pdu[JSON_KEY_ROOMNAME].toString(),respackage2);

    //获取聊天室成员列表
    QVector<Account> members = RoomHelper::getInstance()->handlegetRoomMemberlist(pdu);
    //转发消息给房主
    NetworkHandle::getinstance()->reSend(pdu[JSON_KEY_ROOMNAME].toString(),pdu);
    QVector<Account>::iterator it = members.begin();
    for(; it != members.end(); it++){
        NetworkHandle::getinstance()->reSend(it->getName(),pdu);//转发数据包到所有数据端
    }
}
