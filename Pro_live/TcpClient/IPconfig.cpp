#include "IPconfig.h"
#include "ui_IPconfig.h"
#include <QValidator>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "Log_module/myLog.h"
IPconfig::IPconfig(const QString &ip, quint16 port, QWidget *parent) :
    QWidget(parent),    
    ui(new Ui::IPconfig),
    ip(ip),
    port(port)
{
    ui->setupUi(this);
    ui->lineServerIP->setText(ip);
    ui->line_ServerPort->setText(QString::number(port));
    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    QRegExpValidator* va = new QRegExpValidator(rx,this);
    ui->lineServerIP->setValidator(va);
}

IPconfig::~IPconfig()
{
    delete ui;
}

void IPconfig::on_pb_applyconfig_clicked()
{
    QString qip = ui->lineServerIP->text();
    QString qport = ui->line_ServerPort->text();
    QString path = QDir::currentPath()+QString("/IP.config");
    QFile fd(path);
    if(fd.open(QFile::Truncate|QFile::WriteOnly/*QFile::ReadOnly*/)){
        QTextStream input(&fd);
        input<<qip<<"\r\n"<<qport<<"\r\n";
        QMessageBox::information(this,"配置文件","配置已修改，请重启应用程序");
        QStringList msg;
        msg<<QString("IP.config文件修改：IP-%1;port-%2").arg(qip).arg(qport);
        MYLOG(msg);
    }else{
        QMessageBox::warning(this,"配置文件","配置文件打开失败");
        MYLOG(QStringList("配置文件打开失败"));
    }
    fd.close();
}
