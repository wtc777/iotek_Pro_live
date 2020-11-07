#include "UdpPortHelper.h"

UdpPortHelper::UdpPortHelper()
{
    this->init(10000,10100);//端口池初始化-端口范围
}
UdpPortHelper *UdpPortHelper::instance = nullptr;
UdpPortHelper *UdpPortHelper::getInstance()
{
    if(nullptr == instance){
        instance = new UdpPortHelper;
    }
    return instance;
}

void UdpPortHelper::init(quint16 startport,quint16 endport)
{
    quint16 i = startport;
    for(; i < endport; i++){
        UdpPort temp = {i,false};
        ports.push_back(temp);
    }
}

quint16 UdpPortHelper::assignPort()
{
    QVector<UdpPort>::iterator it = ports.begin();
    for(; it != ports.end(); it++){
        if(it->isUsed == false){
            it->isUsed = true;
            return it->port;
        }
    }
    return 0;
}

void UdpPortHelper::recyclePort(quint16 port)
{
    QVector<UdpPort>::iterator it = ports.begin();
    for(; it != ports.end(); it++){
        if(it->port == port){
            it->isUsed = false;
        }
    }
}
