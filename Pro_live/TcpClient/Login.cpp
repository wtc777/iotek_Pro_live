#include "Login.h"
#include "ui_Login.h"

#include "RoomOpeWidget.h"
#include "LiveOpeWidget.h"
#include "ReceMsgHandle.h"
Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login),
    m_score(0)
{
    ui->setupUi(this);    
    this->setWindowTitle("登录");
    ui->linePwd->setEchoMode(QLineEdit::Password);
    this->loadConfig();
    this->ipconfigW = new IPconfig(this->tcpIP,this->tcpPort,nullptr);
    this->m_tcpsocket = new QTcpSocket;
    this->m_tcpsocket->connectToHost(QHostAddress(this->tcpIP),this->tcpPort);
    connect(m_tcpsocket,SIGNAL(error(QAbstractSocket::SocketError )),
            this,SLOT(onError(QAbstractSocket::SocketError)));
    connect(m_tcpsocket,SIGNAL(readyRead()),this,SLOT(ReadyReadSlot()));

    this->registW = new RegistWidget;
    connect(this->registW, SIGNAL(gobackwidget()),this,SLOT(show()));
}

Login::~Login()
{
    delete this->ipconfigW;
    delete this->m_tcpsocket;
    delete this->registW;
    delete ui;
}

Login *Login::instance = nullptr;
Login *Login::getInstance()
{
    if(nullptr == instance){
        instance = new Login;
    }
    return instance;
}

void Login::loadConfig()
{
    QString path = QDir::currentPath()+QString("/IP.config");
    QFile fd(path);
    if(fd.open(QFile::ReadOnly)){
        QByteArray ba = fd.readAll();
        QString qstr = ba.toStdString().c_str();
        qstr.replace("\r\n"," ");
        QStringList config = qstr.split(" ");
        if(config.size()<2){
            qDebug()<<config.size();
            return ;
        }
        this->tcpIP = config.at(0);
        this->tcpPort = config.at(1).toUShort();
        QStringList msg;
        msg<<"IP配置文件加载成功"<<QString("IP:%1;Port:%2").arg(tcpIP).arg(tcpPort);
        MYLOG(QStringList(msg));
    }else{
        QMessageBox::critical(this,"配置文件","IP配置文件加载失败");
        MYLOG(QStringList("IP配置文件加载失败"));
    }
    fd.close();
}

/*用户注册请求业务逻辑：
*
*
*
*/
void Login::on_btnRegist_clicked()
{
    this->hide();
    this->registW->show();
}

/*用户登录请求业务逻辑：
*
*
*
*/
void Login::on_btnLogin_clicked()
{
    //获取登录信息
    QString qName = ui->lineName->text();
    QString qPwd = ui->linePwd->text();
    if(qName.isEmpty() || qPwd.isEmpty()){
        QMessageBox::warning(this,"登录","密码或用户名为空");
        return;
    }
    //登录信息打包发送
    PDU package(PDU::MSG_TYPE_LOGIN_REQUEST);
    package[JSON_KEY_NAME] = qName;
    package[JSON_KEY_PWD] = qPwd;
    this->m_tcpsocket->write(package.pack());
    this->m_strLoginName = qName;
}

void Login::onError(QAbstractSocket::SocketError socketError)
{
    int errcode = socketError;
    QMessageBox::warning(this,"连接",QString("连接服务器失败:%1").arg(QString::number(errcode))
                                                  +";"+this->m_tcpsocket->errorString());
}

void Login::ReadyReadSlot()
{
    QByteArray array = m_tcpsocket->readAll();
    PDU package;
    int len = 0;
    while((len = package.unpack(array)) > 0){
        //数据包解析并返回数据包整体长度
        ReceMsgHandle rh;
        rh.msgHandle(package);//进入业务逻辑处理判断数据包类型并进行对应的动作
        array = array.mid(len);//获取下一个数据包的数据(粘包情况)
    }
}


void Login::on_pb_config_clicked()
{
    this->ipconfigW->show();
}
