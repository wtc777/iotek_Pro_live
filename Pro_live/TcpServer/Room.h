#ifndef ROOM_H
#define ROOM_H
#include "SocketHandle.h"
#include <QHostAddress>
#include <QUdpSocket>
#include <QObject>
#include "Log_module/myLog.h"
//房间成员类
class Account{
private:
    QString name;
    SocketHandle* socket;
public:
    Account();
    Account(const QString& name);
    Account(const QString& name, SocketHandle* socket);
    //set get
    const QString& getName()const{return this->name;}
    SocketHandle* getSocket()const {return this->socket;}
    //运算符重载 ==
    bool operator==(const Account& rhs){
        return(this->name == rhs.name && this->socket == rhs.socket);
    }
};

//房间类
class Room:public QObject
{
    Q_OBJECT
private:
    QString roomName;               //房主名
    SocketHandle* houseOwnerSocket; //房主套接字    
    QHostAddress groupAddress;      //组播IP地址
    quint16 groupPort;              //组播视频port
    quint16 groupVoicePort;         //组播音频port

    QUdpSocket* sigReceVideoSocket; //单播接收套接字*
    quint16 sigVideoPort;          //单播接收视频port*
    QUdpSocket* groupResendVideoSocket; //组播转发套接字*
    QVector<Account>roomMembers;    //房间成员
public:
    Room(QObject* parent = nullptr);
    explicit Room(QString roomName, SocketHandle* houseOwnerSocket, QObject* parent = nullptr);
    ~Room();
    //set get
    const QString& getRoomName()const {return this->roomName;}

    bool assignGroupAddr();     //组播地址端口号分配
    void givebackGroupPort();   //组播端口归还

    void setgroupPort(quint16 rhs) {this->groupPort = rhs;}
    void setgroupVoicePort(quint16 rhs) {this->groupVoicePort = rhs;}
    void setgroupAddress(const QHostAddress& addr) {this->groupAddress = addr;}

    quint16 getgroupPort()const {return this->groupPort;}
    quint16 getgroupVoicePort()const {return this->groupVoicePort;}
    quint16 getsigVideoPort()const {return this->sigVideoPort;}
    const QHostAddress& getgroupAddress()const {return this->groupAddress;}


    //获取成员列表
    QVector<Account> getRoomMembers()const {return this->roomMembers;}

    //查询成员是否已存在
    bool isMemberExist(Account rhs);
    //添加房间成员
    void addMember(Account rhs) {
        if(!isMemberExist(rhs)){
            roomMembers.push_back(rhs);
        }else{
            return ;
        }
     }
    //删除房间成员
    void eraseMember(Account rhs) {
        QVector<Account>::iterator it = roomMembers.begin();
        for(; it != roomMembers.end(); it++){
            if(it->getName() == rhs.getName()){
                roomMembers.erase(it);
                break;
            }
        }
    }
signals:
private slots:
    void readyResendvideo();//组播转发数据
};

#endif // ROOM_H
