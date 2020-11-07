#ifndef ROOMHELPER_H
#define ROOMHELPER_H
#include "Room.h"
//单例模式--房间操作类
class RoomHelper
{
public:
    static RoomHelper* getInstance();

    //房间操作
    //房间是否存在
    bool handleIsRoomExist(PDU& package)const;
    //创建房间
    int handleCreateRoom(SocketHandle* socket, PDU& package);
    //更新房间列表
    const QVector<QString> handlegetRoomlist()const;
    //加入房间
    bool handleJoinRoom(SocketHandle *socket, PDU&package);
    //更新房间用户列表
    const QVector<Account> handlegetRoomMemberlist(PDU& package)const;
    //房间聊天功能
    void handleRoomChat(PDU&package);
    //退出房间功能
    void handleExitRoom(SocketHandle *handle, PDU&package);

    Room* handleGetRoom(const QString& roomName);
private:
    RoomHelper();
    //禁止拷贝构造函数和运算符值传递操作
    RoomHelper(const RoomHelper&) = delete ;
    const RoomHelper& operator =(const RoomHelper&) = delete ;
    static RoomHelper* instance;

    QVector<Room*>rooms;
};

#endif // ROOMHELPER_H
