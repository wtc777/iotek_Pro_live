#include "ReceMsgHandle.h"
#include "MsgHandle_module/InterfaceMsgHandle.h"
#include "MsgHandle_module/RoomAccessHandle.h"
#include "MsgHandle_module/RoomCreateHandle.h"

ReceMsgHandle::ReceMsgHandle()
{

}


void ReceMsgHandle::msgHandle(PDU& pdu)
{
    Login* mylogin = Login::getInstance();
    InterfaceMsgHandle * ln = nullptr;
    switch (pdu.getPDUtype()) {
    case PDU::MSG_TYPE_REGIST_RESPOND:{
        QMessageBox::information(nullptr,"注册",pdu[JSON_KEY_REGIST_RESPOND].toString());
        break;
    }
    case PDU::MSG_TYPE_LOGIN_SUCCESS_RESPOND:{
        QMessageBox::information(nullptr,"登录提示","登录成功");
        Login::getInstance()->setScore(pdu[JSON_KEY_SCORE].toInt());
        mylogin->hide();
        RoomOpeWidget::getInstance()->flush();
        RoomOpeWidget::getInstance()->show();
        break;
    }
    case PDU::MSG_TYPE_LOGIN_FAILED_RESPOND:{
        QMessageBox::warning(nullptr,"登录提示",pdu[JSON_KEY_LOGIN_RESPOND].toString());
        break;
    }
    case PDU::MSG_TYPE_ROOM_CREATE_RESPOND:{
        ln = new RoomCreateHandle;
        ln->msgHandle(pdu);
        break;
    }
    case PDU::MSG_TYPE_ROOMLIST_FLUSH_RESPOND:{
        RoomOpeWidget::getInstance()->RoomListFlush(pdu);
        break;
    }
    case PDU::MSG_TYPE_ACCESS_ROOM_RESPOND:{
        ln = new RoomAccessHandle;
        ln->msgHandle(pdu);
        break;
    }
    case PDU::MSG_TYPE_FLUSH_ROOM_MEMBERLIST_RESPOND:{
        LiveOpeWidget::getInstance()->FlushMemberLisg(pdu);
        break;
    }
    case PDU::MSG_TYPE_ROOM_CHAT_REQUEST:{
//        qDebug()<<"chat...";
        LiveOpeWidget::getInstance()->FlushMsgTE(pdu);
        break;
    }
    case PDU::MSG_TYPE_ROOM_CHAT_RESPOND:{
        Login::getInstance()->setScore(pdu[JSON_KEY_SCORE].toInt());
        LiveOpeWidget::getInstance()->updateScore();
        break;
    }
    case PDU::MSG_TYPE_EXIT_ROOM_REQUEST:{
        LiveOpeWidget::getInstance()->on_pbExitRoom_clicked();
        break;
    }
    case PDU::MSG_TYPE_VIDEO_DATA:{
//        QByteArray byte = pdu.getPDUobjectArray();
//        LiveOpeWidget::getInstance()->displayVideoimage(byte);
        break;
    }
    case PDU::MSG_TYPE_SEND_GIFT_REQUEST:{//更新礼物弹幕通知
        LiveOpeWidget::getInstance()->FlushMsgTE(pdu);
        LiveOpeWidget::getInstance()->displayGift(pdu[JSON_KEY_GIFE_FILENAME].toString());
        break;
    }
    case PDU::MSG_TYPE_SEND_GIFT_RESPOND:{//更新余额
        Login::getInstance()->setScore(pdu[JSON_KEY_SCORE].toInt());
        LiveOpeWidget::getInstance()->updateScore();
        break;
    }
    default:
        break;
    }
}
