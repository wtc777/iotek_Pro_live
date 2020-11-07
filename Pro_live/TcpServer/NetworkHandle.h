#ifndef NETWORKHANDLE_H
#define NETWORKHANDLE_H

#include <QObject>
#include <QVector>
#include "SocketHandle.h"
#include "Protocol.h"
class NetworkHandle : public QObject
{
    Q_OBJECT
public:

    //set get
    const QVector<SocketHandle*>& getAllclients()const {return this->clients;}
    void addClient(SocketHandle* rhs) {
        clients.push_back(rhs);
        connect(rhs,SIGNAL(currentSocket(SocketHandle*)),this,SLOT(eraseSocket(SocketHandle*)));
    }

    static NetworkHandle* getinstance();

    void reSend(const QString& name, PDU &package);//转发客户端数据包给指定的另一客户端
signals:

public slots:
    void eraseSocket(SocketHandle* socket);
private:
    explicit NetworkHandle(QObject *parent = nullptr);
    NetworkHandle(const NetworkHandle& ) = delete ;
    const NetworkHandle&operator = (const NetworkHandle&) = delete ;

    static NetworkHandle* instance;

    QVector<SocketHandle*> clients;
};

#endif // NETWORKHANDLE_H
