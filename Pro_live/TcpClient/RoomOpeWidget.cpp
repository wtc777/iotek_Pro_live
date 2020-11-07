#include "RoomOpeWidget.h"
#include "ui_RoomOpeWidget.h"
#include <QDebug>
#include "LiveOpeWidget.h"
#include "Login.h"

RoomOpeWidget::RoomOpeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomOpeWidget)
{
    ui->setupUi(this);

    connect(ui->RoomlistLW,SIGNAL(itemDoubleClicked(QListWidgetItem *)),
            this,SLOT(doubleClickedLW(QListWidgetItem *)));
    LiveOpeWidget* room = LiveOpeWidget::getInstance();
    connect(room,SIGNAL(exitRoom()),this,SLOT(show()));

    this->cycFlushRoomList = new QTimer(this);
    connect(this->cycFlushRoomList,SIGNAL(timeout()),
            this,SLOT(on_pbFlushList_clicked()));
    this->cycFlushRoomList->start(1000);//1秒定时刷新房间列表

    waitmovie = new QMovie("://image/gif/waiting.gif",QByteArray(),this);
    waitmovie->setScaledSize(QSize(300,250));
    ui->lb_gif->setMovie(waitmovie);
    waitmovie->start();    
}
RoomOpeWidget *RoomOpeWidget::instance = nullptr;
RoomOpeWidget *RoomOpeWidget::getInstance()
{
    if(nullptr == instance){
        instance = new RoomOpeWidget;
    }
    return instance;
}

void RoomOpeWidget::RoomListFlush(const PDU &package)
{
    ui->RoomlistLW->clear();
    int i = 1;
    for(; i <= package.getObjectSize(); i++){
        QString key = JSON_KEY_ROOMLIST + QString::number(i);
        if(Login::getInstance()->getLoginName() == package[key].toString()){
            continue;
        }
        ui->RoomlistLW->addItem(package[key].toString());
    }
}

void RoomOpeWidget::flush()
{
    //画面显示，状态更新
    this->setWindowTitle(Login::getInstance()->getLoginName());
    ui->lb_score->setText("积分:"+QString::number(Login::getInstance()->getScore()));
}



RoomOpeWidget::~RoomOpeWidget()
{
    delete ui;
}

/*加入房间请求业务逻辑：
*
*
*
*/
void RoomOpeWidget::doubleClickedLW(QListWidgetItem *item)
{
    if(Login::getInstance()->getLoginName() == item->text()){
        LiveOpeWidget::getInstance()->RoomOwnerInit();
        this->hide();
        QString title;
        title = "我的直播间:" + item->text();
        LiveOpeWidget::getInstance()->setWindowTitle(title);
        LiveOpeWidget::getInstance()->show();
        return ;
    }//如果是房主进入直播间则执行画面跳转不向服务器发送请求
    PDU package(PDU::MSG_TYPE_ACCESS_ROOM_REQUEST);
    package[JSON_KEY_ROOMNAME] = item->text();
    Login::getInstance()->getTcpsocket()->write(package.pack());
    LiveOpeWidget::getInstance()->RoomMemberInit();
}

/*创建房间请求业务逻辑：
*
*
*
*/
void RoomOpeWidget::on_pbCreateMyroom_clicked()
{
    QString roomOwmerName = Login::getInstance()->getLoginName();
    PDU package(PDU::MSG_TYPE_ROOM_CREATE_REQUEST);
    package[JSON_KEY_ROOMNAME] = roomOwmerName;
    Login::getInstance()->getTcpsocket()->write(package.pack());
    LiveOpeWidget::getInstance()->RoomOwnerInit();
}

/*房间列表刷新请求业务逻辑：
*
*
*
*/
void RoomOpeWidget::on_pbFlushList_clicked()
{
    if(!this->isHidden()){
        PDU package(PDU::MSG_TYPE_ROOMLIST_FLUSH_REQUEST);
        Login::getInstance()->getTcpsocket()->write(package.pack());
    }
}

