#include "RoomExitHandle.h"

RoomExitHandle::RoomExitHandle()
{

}

void RoomExitHandle::msgHandle(SocketHandle *socket, PDU &package)
{    
    if(package[JSON_KEY_NAME] == package[JSON_KEY_ROOMNAME]){
        //获取聊天室成员列表
        QVector<Account> members = RoomHelper::getInstance()->handlegetRoomMemberlist(package);
        //转发房主退出房间请求-强制房间成员退出直播界面
        QVector<Account>::iterator it = members.begin();
        for(; it != members.end(); it++){
            NetworkHandle::getinstance()->reSend(it->getName(),package);//转发数据包到所有数据端
        }
    }
    //退出房间操作
    RoomHelper::getInstance()->handleExitRoom(socket,package);
}
