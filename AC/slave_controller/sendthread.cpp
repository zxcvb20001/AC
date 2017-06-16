#include "sendthread.h"
#include"global.h"
sendThread *SendFromClient=new sendThread();

sendThread::sendThread()
{

}

void sendThread::_SetRoom(Room *roomFor_temp){//房间类传参
    roomFor=roomFor_temp;
}

//void sendThread::run(){//当连接认证完毕之后，专门用来发送当前温度，当前风速给中央空调；
//    qDebug()<<"根据刷新频率发送温度，风速，设置温度等要求正在准备......";
//    sleep(g_nUpgradeSpeed);//睡眠 刷新频率 的时间
//    QString s="";
//    s=s+"data#1#"+roomID+'#';
//    QString CurrentTem_temp=QString::number(roomFor->_GetCurrentTem(),10);//把当前温度转换为QStirng型
//    s=s+CurrentTem_temp+'#';
//    QString SettedTem_temp=QString::number(roomFor->_GetSettedTem(),10);//把目标温度转换为QString型
//    s=s+SettedTem_temp+'#';
//    QString WorkStyle_temp=QString::number(roomFor->_GetWorkStyle(),10);
//    s=s+WorkStyle_temp+"#;";
//    sendMessage(s);
//}


void sendThread::sendMessage(QString s){
    //int value;
    qDebug()<<"已经发送信息"<<s<<"完毕";
    int k=tcpSocket->write(s.toStdString().c_str(),strlen(s.toStdString().c_str()));
    qDebug()<<"发送返回值为: "<<k;
    //ioctl(tcpSocket,SIOCOUTQ,&value);
    //qDebug()<<"发送第"<<value<<"个socket";

}
