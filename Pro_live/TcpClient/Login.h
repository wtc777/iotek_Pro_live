#ifndef LOGIN_H
#define LOGIN_H
#include "Log_module/myLog.h"
#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFile>
#include <QDir>
#include "Protocol.h"
#include <QDebug>
#include "IPconfig.h"
#include "RegistWidget.h"

class RoomOpeWidget;
class PDU;
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:    
    ~Login();
    static Login* getInstance();
    //  set get
    const QString& getLoginName()const {return this->m_strLoginName;}
    QTcpSocket* getTcpsocket()const {return this->m_tcpsocket;}

    void setScore(int score) { this->m_score = score;}
    int getScore()const {return this->m_score;}

    void loadConfig();
public slots:
    void onError(QAbstractSocket::SocketError socketError);

    void ReadyReadSlot();
private slots:    
    void on_btnRegist_clicked();

    void on_btnLogin_clicked();

    void on_pb_config_clicked();

private:
    explicit Login(QWidget *parent = nullptr);
    static Login* instance;

    Ui::Login *ui;
    QTcpSocket* m_tcpsocket;
    QString tcpIP;
    quint16 tcpPort;
    QString m_strLoginName;
    IPconfig* ipconfigW;
    RegistWidget* registW;
    int m_score;//用户积分
};

#endif // LOGIN_H
