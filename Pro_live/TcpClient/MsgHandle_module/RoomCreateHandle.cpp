#include "RoomCreateHandle.h"

RoomCreateHandle::RoomCreateHandle()
{

}

void RoomCreateHandle::msgHandle(PDU &pdu)
{
    QString respond = pdu[JSON_KEY_SERVER_RESPOND].toString();
    QMessageBox::information(nullptr,"创建房间",respond);
    QString roomName = pdu[JSON_KEY_ROOMNAME].toString();
    if(STR_ROOMCREATE_SUCCESS == respond || STR_ROOMCREATE_EXIST == respond){
        LiveOpeWidget* temp = LiveOpeWidget::getInstance();
        quint16 port = pdu[JSON_KEY_GROUPPORT].toInt();
        QHostAddress ip(pdu[JSON_KEY_GROUPADDRESS].toString());
        quint16 voiceport = pdu[JSON_KEY_GROUPVOICEPORT].toInt();
        qDebug()<<"create room exist ip:"<<ip.toString()<<"port:"<<port;
        temp->setSenderGroupAddr(ip,port,voiceport);
        QString title;
        title = "我的直播间:[" + roomName +
                "];组播IP:[" + temp->getSenderGroupAddr().toString() +
                "];port:[" + QString::number(temp->getSenderGroupPort()) +
                "];voice port:[" + QString::number(temp->getSenderGroupvoicePort()) + "]";
        temp->setRoomName(roomName);
        temp->setWindowTitle(title);
        temp->show();
        RoomOpeWidget::getInstance()->hide();
    }
}
