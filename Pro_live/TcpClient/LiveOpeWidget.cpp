#include "LiveOpeWidget.h"
#include "ui_LiveOpeWidget.h"
#include <QMessageBox>
#include "Protocol.h"
#include "Login.h"
#include <QTime>
#include <QCameraInfo>
#include <QBuffer>
#include <QImageReader>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QScreen>
#include <QMovie>
#include "ReceMsgHandle.h"
LiveOpeWidget::LiveOpeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LiveOpeWidget),
    cvIsopen(false)
{
    ui->setupUi(this);
    startPoint = QPoint(0,0);

    this->sender = new QUdpSocket(this);
    this->giftComboInit();
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    //音频采集对象初始化
    this->voiceInput = new QAudioInput(format);

    //音频输出对象初始化
    this->voiceOutput = new QAudioOutput(format);
    this->voiceOutDevice = this->voiceOutput->start();
    this->receiver = new QUdpSocket(this);       
    connect(this->receiver,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
    this->voiceReceiver = new QUdpSocket(this);
    connect(this->voiceReceiver,SIGNAL(readyRead()),this,SLOT(voiceReadSlot()));
    this->voiceSender = new QUdpSocket(this);
    //视频采集对象初始化
    QCameraInfo info = QCameraInfo::defaultCamera();
    this->cv = new QCamera(info, this);
    this->vs = new VideoSurface(this);
    this->cv->setViewfinder(vs);
    connect(vs,SIGNAL(frameChanged(QVideoFrame)),
            this,SLOT(videoFrameChangedSlot(QVideoFrame)));
    //定时刷新成员列表
    this->cycFlushMemberList = new QTimer(this);
    connect(this->cycFlushMemberList,SIGNAL(timeout()),
            this,SLOT(on_pbFlushMemberlist_clicked()));
    //定时发送屏幕画面
    this->cycSendMyScreen = new QTimer(this);
    connect(this->cycSendMyScreen,SIGNAL(timeout()),
            this,SLOT(displayDesktop()));
    //事件过滤器安装
    ui->lb_ShowVideo->installEventFilter(this);
    ui->lineMsgLE->installEventFilter(this);
}

void LiveOpeWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    on_pbExitRoom_clicked();

}

//事件过滤器：视频显示标签全屏切换显示功能
bool LiveOpeWidget::eventFilter(QObject *ob, QEvent *ev)
{
    if(ob == ui->lb_ShowVideo){
        if(ev->type() == QEvent::MouseButtonDblClick){
            QMouseEvent* mouse = static_cast<QMouseEvent*>(ev);
            if(mouse->button() == Qt::LeftButton){
                //鼠标左键双击进入全屏
                ui->lb_ShowVideo->setWindowFlags(Qt::Dialog);
                ui->lb_ShowVideo->showFullScreen();
                  return true;
            }else if(mouse->button() == Qt::RightButton){
                //右键双击退出全屏
                ui->lb_ShowVideo->setWindowFlags(Qt::SubWindow);
                ui->lb_ShowVideo->showNormal();
                return true;
            }else{
//                return QWidget::eventFilter(ob,ev);
            }
        }
    }else if(ob == ui->lineMsgLE){
        if(ev->type() == QEvent::KeyPress){
            QKeyEvent* key = static_cast<QKeyEvent*>(ev);
            if(key->key() == Qt::Key_Return){
                this->on_pbSendMsg_clicked();
                return true;
            }
        }else{
//            return QWidget::eventFilter(ob,ev);
        }
    }
    return QWidget::eventFilter(ob,ev);
}
LiveOpeWidget *LiveOpeWidget::instance = nullptr;
LiveOpeWidget *LiveOpeWidget::getInstance()
{
    if(nullptr == instance){
        instance = new LiveOpeWidget;
    }
    return instance;
}

LiveOpeWidget::~LiveOpeWidget()
{
    delete this->voiceInput;
    delete this->voiceOutput;
    delete this->voiceInDevice;
    delete this->voiceOutDevice;
    delete ui;
}

void LiveOpeWidget::setSenderGroupAddr(const QHostAddress &rhsAddr, quint16 videoPort, quint16 voicePort)
{
    this->senderGroupAddr = rhsAddr;
    this->senderGroupPort = videoPort;
    this->senderGroupvoicePort = voicePort;
}

void LiveOpeWidget::setReceiverGroupAddr(const QHostAddress &rhsAddr, quint16 videoPort,quint16 voicePort)
{
    this->receiverGroupAddr = rhsAddr;
    this->receiverGroupPort = videoPort;
    this->receiverGroupvoicePort = voicePort;
    //视频组播地址端口绑定
    if(this->receiver->state() == QAbstractSocket::BoundState){
        this->receiver->leaveMulticastGroup(receiverGroupAddr);
        this->receiver->close();
    }
    this->receiver->bind(QHostAddress::AnyIPv4,this->receiverGroupPort,
                         QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    this->receiver->joinMulticastGroup(this->receiverGroupAddr);
    //音频组播地址端口绑定
    if(this->voiceReceiver->state() == QAbstractSocket::BoundState){
        this->voiceReceiver->leaveMulticastGroup(receiverGroupAddr);
        this->voiceReceiver->close();
    }
    this->voiceReceiver->bind(QHostAddress::AnyIPv4,this->receiverGroupvoicePort,
                         QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    this->voiceReceiver->joinMulticastGroup(this->receiverGroupAddr);
}

void LiveOpeWidget::FlushMemberLisg(const PDU &package)
{
    ui->RoomMemberListLW->clear();
    QString roomName = "房主:" + m_strRoomName;
    ui->RoomMemberListLW->addItem(roomName);
    int i = 0;
    for(; i < package.getObjectSize(); i++){
        QString key = JSON_KEY_MEMBERNAME + QString::number(i);
        ui->RoomMemberListLW->addItem(package[key].toString());
    }
}

void LiveOpeWidget::FlushMsgTE(const PDU &package)
{
    if(this->m_strRoomName != package[JSON_KEY_ROOMNAME].toString()){
        return ;
    }
    QString sender = package[JSON_KEY_NAME].toString();
    QString role;
    if(sender == package[JSON_KEY_ROOMNAME].toString()){
        //判断是否是房主
        role = "-房主-";
    }else{
        role = "-观众-";
    }
    QString time = QTime::currentTime().toString();
    sender += role + " " + time + ":";
    QTextCharFormat formatsender;    
    formatsender.setForeground(QBrush(QColor(Qt::blue)));    
    formatsender.setFont(QFont("Consolas",6));
    ui->showMsgTE->setCurrentCharFormat(formatsender);
    ui->showMsgTE->append(sender);

    QTextCharFormat formatMsg;



    formatMsg.setFont(QFont("Consolas",12));
    formatMsg.setForeground(QBrush(QColor(Qt::black)));
    ui->showMsgTE->setCurrentCharFormat(formatMsg);
    ui->showMsgTE->append(package[JSON_KEY_CHATMSG].toString());

    this->displayDanmu(package[JSON_KEY_CHATMSG].toString());
}

void LiveOpeWidget::displayDanmu(const QString &text)
{
    QFont familystyle = ui->fontComboBox->currentFont();
    int size = ui->sizeCB->currentText().toInt();
    familystyle.setPointSize(size);
    QString color = "color:" + ui->colorCB->currentText() + ";";

    QLabel* danmu = new QLabel(text,this);
    danmu->setFont(familystyle);
    danmu->setStyleSheet(color);
    danmu->adjustSize();
    danmu->show();
    int y = startPoint.y();
    y+=20;
    if(y >= this->rect().height()-20){
        y = 0;
    }
    startPoint.setY(y);
    startPoint.setX(this->rect().width());
    startPoint.setX(ui->lb_ShowVideo->rect().width());
    LabelMoveThread* mt = new LabelMoveThread(danmu,startPoint,this);
    this->danmuThreadVector.push_back(mt);
    connect(mt,SIGNAL(moveLabel(const QPoint&,QLabel*)),
            this,SLOT(moveLabel(const QPoint&,QLabel*)));
    connect(mt,SIGNAL(moveComplete(QLabel*)),
            this,SLOT(moveComplete(QLabel*)));
    mt->start();
}

void LiveOpeWidget::displayGift(const QString& filename)
{
    QLabel* gift = new QLabel(this);
    QImage image(filename);
    QPixmap pix = QPixmap::fromImage(image);
    pix = pix.scaled(100,100);
    gift->setPixmap(pix);
    gift->adjustSize();
    gift->show();
    int y = startPoint.y();
    y+=20;
    if(y >= this->rect().height()-20){
        y = 0;
    }
    startPoint.setY(y);
    startPoint.setX(this->rect().width());
    startPoint.setX(ui->lb_ShowVideo->rect().width());
    LabelMoveThread* mt = new LabelMoveThread(gift,startPoint,this);
    this->danmuThreadVector.push_back(mt);
    connect(mt,SIGNAL(moveLabel(const QPoint&,QLabel*)),
            this,SLOT(moveLabel(const QPoint&,QLabel*)));
    connect(mt,SIGNAL(moveComplete(QLabel*)),
            this,SLOT(moveComplete(QLabel*)));
    mt->start();
}

void LiveOpeWidget::updateScore()
{
    int score = Login::getInstance()->getScore();
    ui->lb_score->setText(QString("积分:%1").arg(QString::number(score)));
}

void LiveOpeWidget::init()
{
    this->updateScore();
    on_pbFlushMemberlist_clicked();
    ui->lineMsgLE->clear();
    ui->showMsgTE->clear();
    ui->lb_ShowVideo->setText("摄像头未开启");

    this->cycFlushMemberList->start(3000);//三秒定时刷新房间成员列表
}

void LiveOpeWidget::RoomOwnerInit()
{
    this->init();
    ui->pbSwitchCV->setEnabled(true);
    ui->pbSwitchmicrophone->setEnabled(true);
    ui->pbSendGift->setEnabled(false);
    this->cycSendMyScreen->start(100);
}

void LiveOpeWidget::RoomMemberInit()
{
    this->init();
    ui->pbSwitchCV->setEnabled(false);
    ui->pbSwitchmicrophone->setEnabled(false);
    ui->pbSendGift->setEnabled(true);
}

void LiveOpeWidget::giftComboInit()
{
    ui->comboBoxgift->setIconSize(QSize(100,100));//修改图标尺寸
    //添加礼物列表
    QIcon ico3("://image/递茶.png");
    ui->comboBoxgift->addItem(ico3,"大佬递茶");
    QIcon ico("://image/rocket.png");
    ui->comboBoxgift->addItem(ico,"火箭");    
    QIcon ico1("://image/666.png");
    ui->comboBoxgift->addItem(ico1,"666");
    QIcon ico2("://image/冰阔落.png");
    ui->comboBoxgift->addItem(ico2,"冰阔落");    
}

void LiveOpeWidget::FlushGiftMsg()
{
    QLabel * lb = new QLabel(this);

}

void LiveOpeWidget::sendimageData(QImage image)
{
    //压缩图片
    image = image.scaled(800, 600, Qt::KeepAspectRatio, Qt::FastTransformation).
            scaled(400, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    //图片数据UDP发送
    QByteArray byte;
    QBuffer buff(&byte);
    image.save(&buff,"JPEG",100);
    qDebug()<<"size:"<<byte.size();

    QJsonObject obj = QJsonDocument::fromJson(byte.toBase64()).object();
    qDebug()<<"obj size:"<<obj.size();
    this->sender->writeDatagram(byte,this->senderGroupAddr,this->senderGroupPort);
//    this->sender->writeDatagram(byte,QHostAddress("172.27.24.206"),this->senderGroupPort);
//    PDU package(PDU::MSG_TYPE_VIDEO_DATA);
//    package[JSON_KEY_VIDEO_DATA] = byte.toStdString().c_str();
    //    this->sender->writeDatagram(byte.data(),65000,this->senderGroupAddr,this->senderGroupPort);
}

//void LiveOpeWidget::displayVideoimage(QByteArray &byte)
//{
//    QBuffer buffer(&byte);
//    QImageReader reader(&buffer,"JPEG");
//    QImage image = reader.read();

//    //image-->pixmap
//    QPixmap pix = QPixmap::fromImage(image);
//    pix = pix.scaled(ui->lb_ShowVideo->size());
//    ui->lb_ShowVideo->setPixmap(pix);
//}

/*退出房间请求业务逻辑：
 *  客户端发送退出房间请求(请求方姓名，房间名)->
 *      服务器端判断是否是房主
 *          房主退出房间-服务器发出响应信息给房间成员强制退出直播界面，然后删除房间回收udp端口资源
 *          一般成员退出房间-服务器将成员从房间成员列表中移除。无响应信息给客户端
*/
void LiveOpeWidget::on_pbExitRoom_clicked()
{
    this->hide();
    emit exitRoom();    
    this->cycFlushMemberList->stop();//停止定时刷新成员列表

    if(this->cvIsopen){//如果摄像头已打开则关闭
        this->cv->stop();
        this->cvIsopen = false;
        ui->pbSwitchCV->setText("开启摄像头");
        ui->lb_ShowVideo->setText("摄像头未开启");
    }else{
        this->cycSendMyScreen->stop();
    }
    if(QString("已开启麦克风") == ui->pbSwitchmicrophone->text()){
        this->voiceInput->stop();
        ui->pbSwitchmicrophone->setText("开启麦克风");
    }
    //接收套接字解绑组播地址
    if(this->receiver->state() == QAbstractSocket::BoundState){
        this->receiver->leaveMulticastGroup(receiverGroupAddr);
        this->receiver->close();
    }
    //发送退出房间申请给服务器
    PDU package(PDU::MSG_TYPE_EXIT_ROOM_REQUEST);
    package[JSON_KEY_NAME] = Login::getInstance()->getLoginName();
    package[JSON_KEY_ROOMNAME] = this->m_strRoomName;
    Login::getInstance()->getTcpsocket()->write(package.pack());
    this->m_strRoomName.clear();

    RoomOpeWidget::getInstance()->flush();
}

void LiveOpeWidget::displayDesktop()
{   
    QScreen* desktop = QGuiApplication::primaryScreen();
    QPixmap pix = desktop->grabWindow(0);
    pix = pix.scaled(ui->lb_ShowVideo->size());
    ui->lb_ShowVideo->setPixmap(pix);
    this->sendimageData(pix.toImage());
}



/*直播间发送信息业务逻辑：
*
*
*
*/
void LiveOpeWidget::on_pbSendMsg_clicked()
{
    QString text = ui->lineMsgLE->text();
    if(text.isEmpty()){
        QMessageBox::warning(this,"聊天","发送的信息不能为空");
        return ;
    }
    PDU package(PDU::MSG_TYPE_ROOM_CHAT_REQUEST);
    package[JSON_KEY_ROOMNAME] = this->m_strRoomName;
    package[JSON_KEY_NAME] = Login::getInstance()->getLoginName();
    package[JSON_KEY_CHATMSG] = text;
    Login::getInstance()->getTcpsocket()->write(package.pack());
    ui->lineMsgLE->clear();
}
/*直播间发送信息业务逻辑：
*
*
*
*/
void LiveOpeWidget::on_pbFlushMemberlist_clicked()
{
    PDU package(PDU::MSG_TYPE_FLUSH_ROOM_MEMBERLIST_REQUEST);
    package[JSON_KEY_ROOMNAME] = this->m_strRoomName;
    Login::getInstance()->getTcpsocket()->write(package.pack());
}

void LiveOpeWidget::moveLabel(const QPoint &point, QLabel *lb)
{
    lb->move(point);
    lb->show();
}

void LiveOpeWidget::moveComplete(QLabel *lb)
{
    QVector<LabelMoveThread*>::iterator it = danmuThreadVector.begin();
    for(; it != danmuThreadVector.end(); it++){
        if(lb == (*it)->getLabel()){
            danmuThreadVector.erase(it);
            break;
        }
    }
    lb->close();
}

void LiveOpeWidget::videoFrameChangedSlot(QVideoFrame frame)
{
    //本地显示
    frame.map(QAbstractVideoBuffer::ReadOnly);

    QImage image(
                frame.bits(),
                frame.width(),
                frame.height(),
                frame.bytesPerLine(),
                QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));

    QMatrix matrix;
    matrix.rotate(180);
    image = image.transformed(matrix);

    QPixmap pix = QPixmap::fromImage(image);
    pix = pix.scaled(ui->lb_ShowVideo->size());
    ui->lb_ShowVideo->setPixmap(pix);
    this->sendimageData(image);
}


void LiveOpeWidget::on_pbSwitchCV_clicked()
{    
    if(this->cvIsopen){
        this->cv->stop();
        this->cvIsopen = false;
//        ui->lb_showVideo->hide();
        this->cycSendMyScreen->start(100);
        this->displayDesktop();
        ui->pbSwitchCV->setText("开启摄像头");
    }else{
        this->cv->start();
        this->cvIsopen = true;
        this->cycSendMyScreen->stop();
        ui->pbSwitchCV->setText("摄像头已开启");
    }
}

void LiveOpeWidget::readyReadSlot()
{
#if 0
    qint64 rsize = this->receiver->pendingDatagramSize();
    qDebug()<<"readyReadslot:"<<rsize;
    QByteArray array;
    this->receiver->readDatagram(array.data(),rsize);
    PDU package;
    int len = 0;
    while((len = package.unpack(array)) > 0){
        //数据包解析并返回数据包整体长度
        ReceMsgHandle rh;
        rh.msgHandle(package);//进入业务逻辑处理判断数据包类型并进行对应的动作
        array = array.mid(len);//获取下一个数据包的数据(粘包情况)
    }
#endif
#if 1
    qDebug()<<"udpsocket read...";
    qint64 rsize = this->receiver->pendingDatagramSize();

    QByteArray byte;
    byte.resize(rsize);
    qDebug()<<rsize<<byte.size();
    this->receiver->readDatagram(byte.data(),byte.size());
    QBuffer buffer(&byte);
    QImageReader reader(&buffer,"JPEG");
    QImage image = reader.read();

    //image-->pixmap
    QPixmap pix = QPixmap::fromImage(image);
    pix = pix.scaled(ui->lb_ShowVideo->size());
    ui->lb_ShowVideo->setPixmap(pix);
#endif
}

void LiveOpeWidget::voiceReadSlot()
{
    qDebug()<<"--------->";
    while(this->voiceReceiver->hasPendingDatagrams()){
        qDebug()<<"receive audio...";
        video vp;
        memset(&vp, 0, sizeof(vp));
        this->voiceReceiver->readDatagram((char*)&vp,sizeof(vp));
        if(QString("静音") == ui->pbSilence->text()){
            qDebug()<<"voiceReadSlot receiving...";
            this->voiceOutDevice->write((char*)&vp,sizeof(vp));
        }
    }
}

void LiveOpeWidget::voiceSendSlot()
{
    if(this->isHidden()){
        return;
    }
    qDebug()<<"voiceSendSlot sending...";
    video vp;
    memset(&vp, 0, sizeof(vp));
    vp.len = this->voiceInDevice->read(vp.data,1024);//读取音频
    qDebug()<<vp.len;
    this->voiceSender->writeDatagram((const char*)&vp,sizeof(vp),
                                   this->senderGroupAddr,this->senderGroupvoicePort);
}

void LiveOpeWidget::on_pbSilence_clicked()
{
    if(QString("已静音") == ui->pbSilence->text()){
//        this->voiceOutput->stop();
        ui->pbSilence->setText("静音");
    }else{
//        this->voiceOutput->stop();
        ui->pbSilence->setText("已静音");
    }

}

void LiveOpeWidget::on_pbSwitchmicrophone_clicked()
{
    if(QString("开启麦克风") == ui->pbSwitchmicrophone->text()){
        this->voiceInDevice = this->voiceInput->start();
        connect(this->voiceInDevice,SIGNAL(readyRead()),this,SLOT(voiceSendSlot()));
        ui->pbSwitchmicrophone->setText("已开启麦克风");
    }else{
        this->voiceInput->stop();
        ui->pbSwitchmicrophone->setText("开启麦克风");
    }
}

void LiveOpeWidget::on_pbSendGift_clicked()
{
    int giftNumber = ui->comboBoxgift->currentIndex();
    QString fileName;
    int cost = 0;
    switch (giftNumber) {
    case 0:
        fileName = "://image/递茶.png";
        cost = 20;
        break;
    case 1:
        fileName = "://image/rocket.png";
        cost = 50;
        break;
    case 2:
        fileName = "://image/666.png";
        cost = 60;
        break;
    case 3:
        fileName = "://image/冰阔落.png";
        cost = 30;
        break;
    default:
        break;
    }
    if(fileName.isEmpty()){
        QMessageBox::warning(this,"打赏","未知错误");
        return ;
    }
    if(cost > Login::getInstance()->getScore()){
        QMessageBox::warning(this,"打赏","余额不足");
        return ;
    }
    PDU package(PDU::MSG_TYPE_SEND_GIFT_REQUEST);
    QString sender = Login::getInstance()->getLoginName();
    package[JSON_KEY_NAME] = sender;
    package[JSON_KEY_ROOMNAME] = this->m_strRoomName;
    package[JSON_KEY_GIFE_FILENAME] = fileName;
    package[JSON_KEY_SCORE] = cost;
    package[JSON_KEY_CHATMSG] = sender + "赠送了" + ui->comboBoxgift->currentText();
    Login::getInstance()->getTcpsocket()->write(package.pack());
}
