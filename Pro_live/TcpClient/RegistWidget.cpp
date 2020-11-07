#include "RegistWidget.h"
#include "ui_RegistWidget.h"
#include "Login.h"
RegistWidget::RegistWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistWidget)
{
    ui->setupUi(this);
    ui->linePwd->setEchoMode(QLineEdit::Password);
    ui->linePwdConfirm->setEchoMode(QLineEdit::Password);
    this->setWindowTitle("注册");
}

RegistWidget::~RegistWidget()
{
    delete ui;
}

void RegistWidget::on_pb_regist_clicked()
{
    QString qname = ui->lineName->text();
    QString qpwd = ui->linePwd->text();
    QString qpwd2 = ui->linePwdConfirm->text();
    if(qname.isEmpty() || qpwd.isEmpty()){
        QMessageBox::warning(this,"注册","用户名和密码不能为空");
        return ;
    }

    if(qpwd != qpwd2){
        QMessageBox::warning(this,"注册","两次输入的密码不一致。");
        return ;
    }

    //注册信息打包发送
    PDU package(PDU::MSG_TYPE_REGIST_REQUEST);
    package[JSON_KEY_NAME] = qname;
    package[JSON_KEY_PWD] = qpwd;
    qDebug()<<package[JSON_KEY_NAME].toString()<<package[JSON_KEY_PWD].toString()<<"type"<<package.getPDUtype();
    Login::getInstance()->getTcpsocket()->write(package.pack());
    this->hide();
    emit gobackwidget();
}

void RegistWidget::closeEvent(QCloseEvent *ev)
{
    this->hide();
    emit gobackwidget();
}
