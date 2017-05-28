#include "netcontroller.h"
#include <QDebug>

netController::netController(QObject *parent) :
    QObject(parent)
{
    tsock=new QTcpSocket(this);
    connect(tsock,SIGNAL(readyRead()),this,SLOT(processRecvData()));
    connect(tsock,&QAbstractSocket::disconnected,this,&netController::lostconnect);
}

void netController::sendWindRequest(int roomid,int targettemp,int windspeed){
    if(tsock->isOpen()){
        QChar windchar;
        if(windspeed==0){
            windchar=LOW_WIND_CHAR;
        }
        else if(windspeed){
            windchar=MID_WIND_CHAR;
        }
        else{
            windchar=HIGH_WIND_CHAR;
        }
        tsock->write(QString("-+%1%2%3000000END").arg(roomid).arg(targettemp).arg(windchar).toUtf8());
    }
}

void netController::sendRoomTemp(int roomid,int roomtemp,int windspeed){
    if(tsock->isOpen()){
        QChar windchar;
        if(windspeed==0){
            windchar=LOW_WIND_CHAR;
        }
        else if(windspeed==1){
            windchar=MID_WIND_CHAR;
        }
        else{
            windchar=HIGH_WIND_CHAR;
        }
        tsock->write(QString("##%1%2%3000000END").arg(roomid).arg(roomtemp).arg(windchar).toUtf8());
    }
}

void netController::sendCloseInfo(int roomid){
    if(tsock->isOpen()){
        tsock->write(QString("LE%1:disconnEND").arg(roomid).toUtf8());
        tsock->close();
        tsock->disconnectFromHost();
    }
}

bool netController::preSendOpenInfo(int roomid){
    pendingroomid=roomid;
    if(tsock->state()!=QAbstractSocket::ConnectedState){
        tsock->connectToHost(QHostAddress("127.0.0.1"),5555);//set host ip and port
        if(tsock->waitForConnected(3000)==true){
            sendOpenInfo();
        }
        else{
            return false;
        }
    }
    else{
        sendOpenInfo();
    }
    return true;
}

void netController::sendOpenInfo(){
    tsock->write(QString("@#%1:connectEND").arg(pendingroomid).toUtf8());
}

void netController::processRecvData(){
   while(tsock->bytesAvailable()>0){
       QString serverreplay;
       serverreplay=tsock->readLine();
       if(serverreplay.contains(QString("cold"),Qt::CaseInsensitive)){
           emit recvOpenAck(false);
       }
       else if(serverreplay.contains(QString("warm"),Qt::CaseInsensitive)){
           emit recvOpenAck(true);
       }
       else if(serverreplay.contains(QString("UP"),Qt::CaseInsensitive)){
           emit recvWind(true);
       }
       else if(serverreplay.contains(QString("DOWN"),Qt::CaseInsensitive)){
           emit recvWind(false);
       }
       else if(serverreplay.contains(QString("stop"),Qt::CaseInsensitive)){
           emit compelCLose();
       }
   }
}

void netController::lostconnect(){
    emit compelCLose();
}
