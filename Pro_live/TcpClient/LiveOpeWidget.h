#ifndef LIVEOPEWIDGET_H
#define LIVEOPEWIDGET_H

#include <QWidget>
#include "Protocol.h"
#include "MyThread/LabelMoveThread.h"
#include <QUdpSocket>
#include <QCamera>
#include "VideoSurface.h"
#include <QTimer>
#include <QIODevice>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
namespace Ui {
class LiveOpeWidget;
}

class LiveOpeWidget : public QWidget
{
    Q_OBJECT

public:
    static LiveOpeWidget* getInstance();
    ~LiveOpeWidget();
    //set get
    void setRoomName(const QString& rhs) {this->m_strRoomName = rhs;}
    const QString& getRoomName()const {return this->m_strRoomName;}

    void setSenderGroupAddr(const QHostAddress&rhsAddr, quint16 videoPort,quint16 voicePort);
    void setReceiverGroupAddr(const QHostAddress&rhsAddr, quint16 videoPort, quint16 voicePort);

    const QHostAddress& getSenderGroupAddr()const {return this->senderGroupAddr;}
    quint16 getSenderGroupPort()const {return this->senderGroupPort;}
    quint16 getSenderGroupvoicePort()const {return this->senderGroupvoicePort;}

    const QHostAddress& getReceiverGroupAddr()const {return this->receiverGroupAddr;}
    quint16 getReceiverGroupPort()const {return this->receiverGroupPort;}
    quint16 getReceiverGroupvoicePort()const {return this->receiverGroupvoicePort;}

    void FlushMemberLisg(const PDU& package);

    void FlushMsgTE(const PDU& package);

    void displayDanmu(const QString&text);    

    void displayGift(const QString &filename);//显示礼物

    void updateScore();//更新用户积分

    void init();

    void RoomOwnerInit();

    void RoomMemberInit();

    void giftComboInit();//礼物盒初始化

    void FlushGiftMsg();  //显示礼物赠送特效

    void sendimageData(QImage byte);

//    void displayVideoimage(QByteArray& byte);

signals:
    void exitRoom();
public slots:
    void on_pbExitRoom_clicked();

    void displayDesktop();//分享屏幕

private slots:    

    void on_pbSendMsg_clicked();

    void on_pbFlushMemberlist_clicked();

    void moveLabel(const QPoint& point,QLabel* lb);//弹幕移动处理

    void moveComplete(QLabel* lb);  //弹幕移动结束处理

    void videoFrameChangedSlot(QVideoFrame frame);

    void on_pbSwitchCV_clicked();

    void readyReadSlot();

    void voiceReadSlot();

    void voiceSendSlot();

    void on_pbSilence_clicked();

    void on_pbSwitchmicrophone_clicked();


    void on_pbSendGift_clicked();

private:
    typedef struct {
        char data[1024];
        int len;
    }video;
    explicit LiveOpeWidget(QWidget *parent = nullptr);
    LiveOpeWidget(const LiveOpeWidget&) = delete ;
    const LiveOpeWidget& operator =(const LiveOpeWidget&) = delete ;

    static LiveOpeWidget* instance;
    Ui::LiveOpeWidget *ui;

    QString m_strRoomName;
    QVector<LabelMoveThread*> danmuThreadVector;
    QPoint startPoint;//弹幕起始坐标

    QUdpSocket* sender;
    QUdpSocket* receiver;
    QUdpSocket* voiceSender;
    QUdpSocket* voiceReceiver;

    QHostAddress senderGroupAddr;
    quint16 senderGroupPort;
    quint16 senderGroupvoicePort;
    QAudioInput* voiceInput;
    QIODevice* voiceInDevice;

    QHostAddress receiverGroupAddr;
    quint16 receiverGroupPort;
    quint16 receiverGroupvoicePort;
    QAudioOutput* voiceOutput;
    QIODevice* voiceOutDevice;

    QCamera* cv;    //摄像机对象
    VideoSurface* vs;//摄像图像处理对象

    bool cvIsopen;

    QTimer* cycFlushMemberList;//定时刷新成员列表
    QTimer* cycSendMyScreen;    //定时发送主播屏幕画面


protected:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject* ob,QEvent* ev);
};

#endif // LIVEOPEWIDGET_H
