#include "RoomAccessHandle.h"

RoomAccessHandle::RoomAccessHandle()
{

}

void RoomAccessHandle::msgHandle(PDU &pdu)
{
    QString respond = pdu[JSON_KEY_SERVER_RESPOND].toString();
    if(STR_ACCESSROOM_SUCCESS == respond){
        quint16 port = pdu[JSON_KEY_GROUPPORT].toInt();
        QHostAddress ip(pdu[JSON_KEY_GROUPADDRESS].toString());
        quint16 voiceport = pdu[JSON_KEY_GROUPVOICEPORT].toInt();
        LiveOpeWidget::getInstance()->setReceiverGroupAddr(ip,port,voiceport);

        QMessageBox::information(nullptr,"直播间",respond);
        QString title = "用户:[" + Login::getInstance()->getLoginName();
        title += "];直播间:[" + pdu[JSON_KEY_ROOMNAME].toString();
        LiveOpeWidget* temp = LiveOpeWidget::getInstance();
        title += ";]组播IP:[" + temp->getReceiverGroupAddr().toString() +
                "];port:[" + QString::number(temp->getReceiverGroupPort()) +
                "];voice port:[" + QString::number(temp->getReceiverGroupvoicePort()) + "]";
        temp->setRoomName(pdu[JSON_KEY_ROOMNAME].toString());
        temp->setWindowTitle(title);
        temp->show();
        RoomOpeWidget::getInstance()->hide();
    }else if(STR_ACCESSROOM_REFUSED == respond){
        QMessageBox::warning(nullptr,"直播间",respond);
    }
}
