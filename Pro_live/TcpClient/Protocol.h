#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <cstring>
#include <cstdlib>
#include <QJsonDocument>
#include <QJsonObject>
#if 0
变长协议包类的数据传递过程为
打包过程是
    将数据体部QJsonObject转换为QByteArray+头部的（数据包类型及实际包长度）
解析部分就是
    将socket读取的QByteArray类型数据拆分成头部和体部。
    体部转换为QJsonObject放到PDU类的PDUobject成员中。头部的数据类型部分放到PDU类的PDUtype成员中。
数据类型的转换过程中使用了QJsonDocument类的两个方法
    QJsonObject转化为QByteArray使用的是QJsonDocument的tojson方法
    反向转换则使用使用的是QJsonDocument::fromJson().object();
#endif

#define JSON_KEY_REGIST_RESPOND "regist respond"
#define JSON_KEY_LOGIN_RESPOND "login respond"
#define JSON_KEY_NAME "name"
#define JSON_KEY_PWD "pwd"
#define JSON_KEY_ROOMLIST "room list"
#define JSON_KEY_CHATMSG "room chat message"
#define JSON_KEY_SERVER_RESPOND "server respond"
#define JSON_KEY_ROOMNAME   "room name"
#define JSON_KEY_MEMBERNAME "room member name"
#define JSON_KEY_GROUPADDRESS "group address"
#define JSON_KEY_GROUPPORT  "group port"
#define JSON_KEY_GROUPVOICEPORT "group voice port"
#define JSON_KEY_SIGVIDEOPORT "sig video port"
#define JSON_KEY_VIDEO_DATA "video data"
#define JSON_KEY_VOICE_DATA "voice data"
#define JSON_KEY_SCORE "score"
#define JSON_KEY_GIFE_FILENAME "gift filename"

#define STR_UNKNOWN_ERROR   "server unknown error"
#define STR_ROOMCREATE_SUCCESS "create room success"
#define STR_ROOMCREATE_EXIST    "room already exist"
#define STR_ACCESSROOM_SUCCESS  "access room success"
#define STR_ACCESSROOM_REFUSED  "access room refused or room not exist"

class PDU{
public:
    enum MSG_TYPE{
        MSG_TYPE_NONE,

        MSG_TYPE_REGIST_REQUEST,                                        //注册请求
        MSG_TYPE_REGIST_RESPOND,                                        //注册响应

        MSG_TYPE_LOGIN_REQUEST,                                         //登录请求
        MSG_TYPE_LOGIN_SUCCESS_RESPOND,                                 //登录成功响应
        MSG_TYPE_LOGIN_FAILED_RESPOND,                                  //登录失败响应

        MSG_TYPE_ROOM_CHAT_REQUEST,                                     //房间聊天请求
        MSG_TYPE_ROOM_CHAT_RESPOND,                                     //房间聊天响应

        MSG_TYPE_ROOM_CREATE_REQUEST,                                   //创建房间请求
        MSG_TYPE_ROOM_CREATE_RESPOND,                                   //创建房间响应

        MSG_TYPE_ROOMLIST_FLUSH_REQUEST,                                //刷新房间列表请求
        MSG_TYPE_ROOMLIST_FLUSH_RESPOND,                                //刷新房间列表响应

        MSG_TYPE_ACCESS_ROOM_REQUEST,                                   //进入房间请求
        MSG_TYPE_ACCESS_ROOM_RESPOND,                                   //进入房间响应

        MSG_TYPE_FLUSH_ROOM_MEMBERLIST_REQUEST,                         //刷新房间成员列表请求
        MSG_TYPE_FLUSH_ROOM_MEMBERLIST_RESPOND,                         //刷新房间成员列表响应

        MSG_TYPE_EXIT_ROOM_REQUEST,                                     //退出房间请求
        MSG_TYPE_EXIT_ROOM_RESPOND,                                     //退出房间响应

        MSG_TYPE_SEND_GIFT_REQUEST,                                     //给主播送礼请求
        MSG_TYPE_SEND_GIFT_RESPOND,                                     //给主播送礼响应

        MSG_TYPE_VIDEO_DATA,                                            //视频数据类型
        MSG_TYPE_VOICE_DATA,                                            //音频数据类型

        MSG_TYPE_MAX = 0X00FFFFFF,
    };

    PDU(MSG_TYPE type = MSG_TYPE_NONE);

    //set get
    PDU::MSG_TYPE getPDUtype()const {return this->PDUtype;}
    void setPDUtype(PDU::MSG_TYPE type) {this->PDUtype = type;}

    int getObjectSize()const {return PDUobject.size();}

    //运算符重载.对包体的访问方法[]
    QJsonValueRef operator [](const QString& key){return this->PDUobject[key];}//引用-用于修改包体中的值
    QJsonValue operator[](const QString& key)const {return this->PDUobject[key];}//用于获取包体中的值

    //协议包封装
    QByteArray pack();
    //数据包解包
    int unpack(QByteArray array);

private:
    MSG_TYPE PDUtype;//head:协议包类型
    QJsonObject PDUobject;//body:数据包内容

};

#endif // PROTOCOL_H
