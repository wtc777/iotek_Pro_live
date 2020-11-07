#include "NetworkHandle.h"
#include "OpeDB.h"

NetworkHandle *NetworkHandle::instance = nullptr;
NetworkHandle *NetworkHandle::getinstance()
{
    if(nullptr == instance)
        instance = new NetworkHandle;
    return instance;
}

void NetworkHandle::reSend(const QString &name, PDU &package)
{
    if(name.isEmpty())
        return ;
    QVector<SocketHandle*>::iterator it = clients.begin();
    for(; it != clients.end(); it++){
        SocketHandle* temp = *it;
        if(name == temp->getName()){
            temp->getClientsocket()->write(package.pack());
        }
    }
}

void NetworkHandle::eraseSocket(SocketHandle *socket)
{
    QVector<SocketHandle*>::iterator it = clients.begin();
    for(it = clients.begin(); it != clients.end(); it++){
        if(*it == socket){
            OpeDB::getInstance()->handleOffline(socket->getName().toStdString().c_str());
            clients.erase(it);
            break;
        }
    }
}

NetworkHandle::NetworkHandle(QObject *parent) : QObject(parent)
{

}
