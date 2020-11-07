#include "Protocol.h"
#include <QDebug>

PDU::PDU(PDU::MSG_TYPE type)
    :PDUtype(type)
{

}

QByteArray PDU::pack()
{
    QByteArray head(8,0);   //初始化8字节的头部：数据包类型+包体（PDUobject）数据长度
    *(PDU::MSG_TYPE*)head.data() = this->PDUtype;
    QByteArray body = QJsonDocument(PDUobject).toJson();//将数据包体转化为QByteArray类型
    *(int*)(head.data()+4) = body.size();//获取数据包体的长度
    return head+body;//返回打包后的QByteArray类型的数据包
}

int PDU::unpack(QByteArray array)
{
    if(array.size()<8){
        return 0;//数据包头部不完整小于8个字节(数据已读取完成)则返回
    }
    this->PDUtype = *(PDU::MSG_TYPE*)array.data();//获取数据包类型信息
    int len = *(int*)(array.data()+4);  //获取实际数据包体长度
    if(array.size() < len + 8){
        return 0;   //如果实际数据包大小 小于 数据包头部的数据包长度信息(半包) 则返回
    }
    QJsonObject temp = QJsonDocument::fromJson(array.mid(8,len)).object();//获取实际数据包体部内容并由QByteArray转换为qjsonobject类型
//    if(temp.isEmpty()){
//        return 0;
//    }
    this->PDUobject = temp;
    return len + 8;//解析完成返回本次数据包的总长度
}
