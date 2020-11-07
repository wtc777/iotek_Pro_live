#include "LoginHandle.h"

LoginHandle::LoginHandle()
{

}

void LoginHandle::msgHandle(SocketHandle *handle, PDU &pdu)
{
    QString caName = pdu[JSON_KEY_NAME].toString();
    QString caPwd = pdu[JSON_KEY_PWD].toString();
    bool ret = OpeDB::getInstance()->handleLogin(caName.toStdString().c_str(),
                                                caPwd.toStdString().c_str());
    PDU rePackage;
    if(ret){
        rePackage.setPDUtype(PDU::MSG_TYPE_LOGIN_SUCCESS_RESPOND);
        //获取用户积分余额
        rePackage[JSON_KEY_SCORE] = OpeDB::getInstance()->handleGetScore(caName.toStdString().c_str());
        handle->setName(caName);
    }else{
        rePackage.setPDUtype(PDU::MSG_TYPE_LOGIN_FAILED_RESPOND);
        rePackage[JSON_KEY_LOGIN_RESPOND] = QString("relogin or name error or pwd error");
    }
    handle->getClientsocket()->write(rePackage.pack());
}
