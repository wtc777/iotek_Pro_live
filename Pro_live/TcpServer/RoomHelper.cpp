#include "RoomHelper.h"
#include "Protocol.h"
#include "UdpPortHelper.h"
RoomHelper *RoomHelper::instance = nullptr;
RoomHelper *RoomHelper::getInstance()
{
    if(nullptr == instance){
        instance = new RoomHelper;
    }
    return instance;
}

bool RoomHelper::handleIsRoomExist(PDU &package) const
{
    QString roomName = package[JSON_KEY_ROOMNAME].toString();
    QVector<Room*>::const_iterator  cit = this->rooms.cbegin();
    for(; cit != rooms.cend(); cit++){
        Room* temp = *cit;
        if(roomName == temp->getRoomName())
            return true;
    }
    return false;
}

int RoomHelper::handleCreateRoom(SocketHandle *socket, PDU &package)
{
    if(handleIsRoomExist(package)){//房间已存在
        return 1;
    }else{//房间不存在,分配组播地址，音视频端口号
        Room* room = new Room(package[JSON_KEY_ROOMNAME].toString(),socket);
        bool ret = room->assignGroupAddr();
        if(!ret){
            return 2;//组播端口获取失败
        }
        this->rooms.push_back(room);
        return 0;
    }
}

const QVector<QString> RoomHelper::handlegetRoomlist() const
{
    QVector<Room*>::const_iterator  cit = this->rooms.cbegin();
    QVector<QString>roomList;
    for(; cit != rooms.cend(); cit++){
        Room* temp = *cit;
        QString roomName = temp->getRoomName();
        roomList.push_back(roomName);
    }
    return roomList;
}

bool RoomHelper::handleJoinRoom(SocketHandle* socket, PDU &package)
{
    QString roomName = package[JSON_KEY_ROOMNAME].toString();
    if(!handleIsRoomExist(package)){//房间不存在
        return false;
    }else{
        Account member(socket->getName(),socket);
        QVector<Room*>::iterator it = this->rooms.begin();
        for(; it != rooms.end(); it++){
            Room* temp = *it;
            if(temp->getRoomName() == roomName){
                temp->addMember(member);
                return true;
            }
        }
        return false;
    }
}

const QVector<Account> RoomHelper::handlegetRoomMemberlist(PDU &package) const
{
    QString roomName = package[JSON_KEY_ROOMNAME].toString();
    QVector<Room*>::const_iterator cit = rooms.cbegin();
    QVector<Account> res;
    for(; cit != rooms.cend(); cit++){
        if(roomName == (*cit)->getRoomName()){
            res = (*cit)->getRoomMembers();
            break;
        }
    }
    return res;
}

void RoomHelper::handleExitRoom(SocketHandle* handle, PDU &package)
{
    //获取房间
    QString roomName = package[JSON_KEY_ROOMNAME].toString();
    QVector<Room*>::iterator  it = this->rooms.begin();
    for(; it != rooms.end(); it++){
        Room* temp = *it;
        if(roomName == temp->getRoomName()){
            if(package[JSON_KEY_NAME] == package[JSON_KEY_ROOMNAME]){//房主退出房间-删除房间回收端口资源
                QString roomName = package[JSON_KEY_ROOMNAME].toString();
                //获取房间视频组播端口号,回收音频组播端口
                RoomHelper::getInstance()->handleGetRoom(roomName)->givebackGroupPort();
                rooms.erase(it);    //删除房间
            }else{//删除房间成员
                Account person(package[JSON_KEY_NAME].toString(),handle);
                (*it)->eraseMember(person);
            }
            break;
        }
    }
}

Room *RoomHelper::handleGetRoom(const QString &roomName)
{
    QVector<Room*>::iterator cit = this->rooms.begin();
    Room* temp = nullptr;
    for(; cit != rooms.end(); cit++){
        if(roomName == (*cit)->getRoomName()){
            temp = *cit;
        }
    }
    return temp;
}

RoomHelper::RoomHelper()
{

}
