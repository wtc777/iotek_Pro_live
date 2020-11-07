#ifndef UDPPORTHELPER_H
#define UDPPORTHELPER_H
#include <QVector>

class UdpPortHelper
{
private:
    UdpPortHelper();
    UdpPortHelper(const UdpPortHelper&) = delete;
    const UdpPortHelper& operator = (const UdpPortHelper&) = delete;
    static UdpPortHelper* instance;

    //自定义端口类型
    typedef struct typeUdpPort{
        quint16 port;
        bool isUsed;
    }UdpPort;

    QVector<UdpPort> ports;
public:
    static UdpPortHelper* getInstance();
    void init(quint16 startport, quint16 endport);

    quint16 assignPort();//分配端口
    void recyclePort(quint16 port);//回收端口
};

#endif // UDPPORTHELPER_H
