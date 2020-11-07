#ifndef SOCKETHANDLE_H
#define SOCKETHANDLE_H

#include <QObject>
#include <QTcpSocket>
class PDU;
class SocketHandle : public QTcpSocket
{
    Q_OBJECT
public:
    explicit SocketHandle(QTcpSocket* clientsocket, QObject *parent = nullptr);
    //set get
    QTcpSocket* getClientsocket()const {return this->clientsocket;}
    const QString& getName()const {return this->m_strName;}
    void setName(const QString& rhs) {this->m_strName = rhs;}
signals:
    void currentSocket(SocketHandle* socket);
public slots:
    void ReadyReadSlot();
    void disconnectSlot();

private:
    QTcpSocket* clientsocket;
    QString m_strName;
};

#endif // SOCKETHANDLE_H
