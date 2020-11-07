#include "RegistHandle.h"

RegistHandle::RegistHandle()
{

}

void RegistHandle::msgHandle(SocketHandle *handle, PDU &pdu)
{
    QString caName = pdu[JSON_KEY_NAME].toString();
    QString caPwd = pdu[JSON_KEY_PWD].toString();
    qDebug()<<pdu[JSON_KEY_NAME].toString()<<pdu[JSON_KEY_PWD].toString()<<"handle regist";
    bool ret = OpeDB::getInstance()->handleRegist(caName.toStdString().c_str(),
                caPwd.toStdString().c_str());
    PDU rePackage(PDU::MSG_TYPE_REGIST_RESPOND);
    if(ret){
        //注册成功
        rePackage[JSON_KEY_REGIST_RESPOND] = QString("注册成功");
    }else{
        //注册失败
        rePackage[JSON_KEY_REGIST_RESPOND] = QString("注册失败");
    }    handle->getClientsocket()->write(rePackage.pack());
}
