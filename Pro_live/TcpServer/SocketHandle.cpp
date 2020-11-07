#include "SocketHandle.h"
#include "NetworkHandle.h"
#include "Protocol.h"
#include <QDebug>
#include "OpeDB.h"
#include "ReceMsgHandle.h"

SocketHandle::SocketHandle(QTcpSocket *clientsocket, QObject *parent)
    : QTcpSocket(parent),
      clientsocket(clientsocket)
{
    connect(clientsocket,SIGNAL(readyRead()),this,SLOT(ReadyReadSlot()));
    connect(clientsocket,SIGNAL(disconnected()),this,SLOT(disconnectSlot()));
    connect(clientsocket,SIGNAL(disconnected()),this,SLOT(deleteLater()));
}


void SocketHandle::ReadyReadSlot()
{
    QByteArray array = this->clientsocket->readAll();
    PDU package;
    int len = 0;
    int i = 0;
    while((len = package.unpack(array)) > 0){//数据包解析并返回数据包整体长度
        qDebug()<<"receive message";
        ReceMsgHandle receHandle;
        receHandle.MsgHandle(this,package);//进入业务逻辑处理判断数据包类型并进行对应的动作
        array = array.mid(len);//获取下一个数据包的数据(粘包情况)
        i++;
        qDebug()<<"长度："<<len<<"循环："<<i;
    }
}

void SocketHandle::disconnectSlot()
{
    emit currentSocket(this);
}


