#include "Room.h"
#include "UdpPortHelper.h"

Room::Room(QObject* parent)
    :QObject (parent)
{

}

Room::Room(QString roomName, SocketHandle *houseOwnerSocket, QObject *parent)
    :QObject (parent),
      roomName(roomName),
      houseOwnerSocket(houseOwnerSocket)
{
    this->sigReceVideoSocket = new QUdpSocket;
    this->groupResendVideoSocket = new QUdpSocket;
    connect(this->sigReceVideoSocket,SIGNAL(readyRead()),this,SLOT(readyResendvideo()));
}
Room::~Room()
{
    delete this->sigReceVideoSocket;
    delete this->groupResendVideoSocket;

}

bool Room::assignGroupAddr()
{
    UdpPortHelper* helper = UdpPortHelper::getInstance();
//    this->groupAddress = QHostAddress("224.0.0.87");
    this->groupAddress = QHostAddress("224.0.1.88");
    if((this->groupPort = helper->assignPort()) == 0){
        return false;
    }
    if((this->groupVoicePort = helper->assignPort()) == 0){
        return false;
    }
    if((this->sigVideoPort = helper->assignPort()) == 0){
        return false;
    }
    //绑定udp单播接收端地址
    if(this->sigReceVideoSocket->state() == QAbstractSocket::BoundState){
        this->sigReceVideoSocket->close();
    }
    this->sigReceVideoSocket->bind(/*QHostAddress::AnyIPv4,*/this->sigVideoPort,
                                   QAbstractSocket::ShareAddress|QAbstractSocket::ReuseAddressHint);

    return true;
}

void Room::givebackGroupPort()
{
    UdpPortHelper* helper = UdpPortHelper::getInstance();
    helper->recyclePort(this->groupPort);
    helper->recyclePort(this->groupVoicePort);
    helper->recyclePort(this->sigVideoPort);
}

bool Room::isMemberExist(Account rhs)
{
    QVector<Account>::iterator it = roomMembers.begin();
    for(; it != roomMembers.end(); it++){
        if(rhs == *it){
            return true;
        }
    }
    return false;
}

void Room::readyResendvideo()
{
    qDebug()<<"readyResendvideo...";
    MYLOG(QStringList("readyResendvideo..."));
    qint64 rsize = this->sigReceVideoSocket->pendingDatagramSize();

    QByteArray byte;
    byte.resize(rsize);
    qDebug()<<rsize<<byte.size();
    this->sigReceVideoSocket->readDatagram(byte.data(),byte.size());
//组播转发
    this->groupResendVideoSocket->writeDatagram(byte,
                                                this->groupAddress,this->groupPort);
}

Account::Account()
{

}

Account::Account(const QString &name)
    :name(name)
{

}

Account::Account(const QString &name, SocketHandle *socket)
    :name(name),
      socket(socket)
{

}
