#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include "NetworkHandle.h"
#include "SocketHandle.h"
#include "DBHelper.h"
#include "OpeDB.h"
#include "Log_module/myLog.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegExpValidator>
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->loadconfig();
    ui->lineIp->setText(this->IP);
    ui->linePort->setText(QString::number(this->port));
    //正则表达式限制IP地址输入格式为 x.x.x.x
    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    QRegExpValidator* va = new QRegExpValidator(rx,this);
    ui->lineIp->setValidator(va);

    this->m_tcpserver = new QTcpServer;

    connect(m_tcpserver,SIGNAL(newConnection()),this,SLOT(newconnectSlot()));
    on_btnListen_clicked();
    OpeDB::getInstance()->handleAlloffline();
}

void MainWindow::loadconfig()
{
    QString fileName = QDir::currentPath() + "/IP.config";
    QFile fd(fileName);
    if(fd.open(QFile::ReadOnly)){
        QByteArray ary = fd.readAll();
        QString text = ary.data();
        text.replace("\r\n"," ");
        QStringList addr = text.split(" ");
        if(addr.size() > 2){
            this->IP = addr.at(0);
            this->port = addr.at(1).toInt();
        }
        MYLOG(QStringList(QString("配置文件加载成功,监听地址IP:%1;Port:%2;")
                          .arg(this->IP).arg(QString::number(this->port))));
    }else{
        QMessageBox::critical(this,"配置文件","IP配置文件打开失败");
        this->IP = "127.0.0.1";
        this->port = 5555;
        MYLOG(QStringList(QString("配置文件加载失败,监听地址IP:%1;Port:%2;")
                          .arg(this->IP).arg(QString::number(this->port))));
    }
}

MainWindow::~MainWindow()
{
    delete this->m_tcpserver;
    delete ui;    
}


void MainWindow::on_btnListen_clicked()
{
    if(this->m_tcpserver->isListening()){
        ui->te_show->append("已经监听");
        return ;
    }
    bool ret = this->m_tcpserver->listen(QHostAddress(this->IP),this->port);
    if(ret){
        ui->te_show->append("监听成功");
    }else{
        ui->te_show->append("监听失败");
    }
}

void MainWindow::newconnectSlot()
{    
    QTcpSocket* socket = this->m_tcpserver->nextPendingConnection();
    SocketHandle* sh = new SocketHandle(socket,this);
    NetworkHandle::getinstance()->addClient(sh);//保存客户端套接字到容器
    QString ip = socket->peerAddress().toString();
    quint16 port = socket->peerPort();
    ui->te_show->append(ip);
    ui->te_show->append(QString::number(port));
    QStringList msg;
    msg<<"new connection"<<ip<<QString::number(port);
    MYLOG(msg);
}



void MainWindow::on_pb_showLog_clicked()
{
    myLog::getInstance()->show();
}

void MainWindow::on_pb_changeIP_clicked()
{
    QString qIP = ui->lineIp->text();
    QString qport = ui->linePort->text();
    if(qIP.isEmpty() || qport.isEmpty()){
        QMessageBox::warning(this,"配置IP","ip和port不能为空");
        return ;
    }
    QString fileName = QDir::currentPath() + "/IP.config";
    QFile fd(fileName);
    if(fd.open(QFile::WriteOnly | QFile::Truncate)){
        QTextStream out(&fd);
        out<<qIP<<"\r\n"<<qport<<"\r\n";
        QMessageBox::information(this,"配置文件","IP地址已修改，请重启服务端程序,生效配置");
    }else{
        QMessageBox::warning(this,"配置文件","IP配置文件打开失败.修改终止");
        return ;
    }

}
