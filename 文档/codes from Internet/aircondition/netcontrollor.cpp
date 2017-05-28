#include "netcontrollor.h"

NetControllor::NetControllor(QObject *parent) :
    QObject(parent)
{
    mainserver=new QTcpServer(this);
    connect(mainserver,&QTcpServer::newConnection,this,&NetControllor::processNewConnection);
}

void NetControllor::startserver(){
    mainserver->listen(QHostAddress("127.0.0.1"),5555);//set ip and port
}

void NetControllor::stopserver(){
    mainserver->close();
}

void NetControllor::sendMode(int roomid, bool currentmode){
    if(roomsocketmap.contains(roomid)){
        if(currentmode==COLDMODE){
            roomsocketmap[roomid]->write(QString("Air is cold model").toUtf8());
        }
        else{
            roomsocketmap[roomid]->write(QString("Air is warm model").toUtf8());
        }
    }
}

void NetControllor::sendWind(int roomid,bool windtype){

    if(roomsocketmap.contains(roomid)){
        if(windtype==COLD_WIND){
            roomsocketmap[roomid]->write(QString("DOWNDOWN").toUtf8());
        }
        else{
            roomsocketmap[roomid]->write(QString("UPUP").toUtf8());
        }
    }
}

void NetControllor::sendCompelStop(int roomid){
    if(roomsocketmap.contains(roomid)){
        roomsocketmap[roomid]->write(QString("Server makes you stop").toUtf8());
        roomsocketmap[roomid]->disconnectFromHost();
        roomsocketmap.remove(roomid);
    }
}

void NetControllor::parseData(QString &recvdata){
    int roomid;
    int windspeed;
    int roomtemp;
    int targettemp;
    if(recvdata[11]!='E'||recvdata[12]!='N'||recvdata[13]!='D'){
        return;
    }
    if(recvdata[0]=='@'){
        roomid=recvdata.mid(2,1).toInt();
        emit subMachineStart(roomid);
    }
    else if(recvdata[0]=='#'){
        roomid=recvdata.mid(2,1).toInt();
        roomtemp=recvdata.mid(3,2).toInt();
        windspeed=recvdata.mid(5,1).toInt();
        emit updateRoomTemp(roomid,roomtemp,windspeed);
    }
    else if(recvdata[0]=='-'){
        roomid=recvdata.mid(2,1).toInt();
        targettemp=recvdata.mid(3,2).toInt();
        windspeed=recvdata.mid(5,1).toInt();
        emit subMachineRequestWind(roomid,targettemp,windspeed);
    }
    else if(recvdata[0]=='L'){
        roomid=recvdata.mid(2,1).toInt();
        emit subMachineStop(roomid);
    }
}

int NetControllor::getRoomID(QString &recvdata){
    int roomid;
    if(recvdata[11]!='E'||recvdata[12]!='N'||recvdata[13]!='D'){
        return -1;
    }
    if(recvdata[0]=='@'){
        roomid=recvdata.mid(2,1).toInt();
        return roomid;
    }
    else if(recvdata[0]=='#'){
        roomid=recvdata.mid(2,1).toInt();
        return roomid;
    }
    else if(recvdata[0]=='-'){
        roomid=recvdata.mid(1,1).toInt();
        return roomid;
    }
    else if(recvdata[0]=='+'){
        roomid=recvdata.mid(1,1).toInt();
        return roomid;
    }
    else if(recvdata[0]=='L'){
        roomid=recvdata.mid(2,1).toInt();
        return roomid;
    }

    return -1;
}

void NetControllor::processData(){

    QString recvdata;
    for(QMap<int,QTcpSocket*>::iterator i=roomsocketmap.begin();i!=roomsocketmap.end();++i){
        while(i.value()->bytesAvailable()>0){
            recvdata=i.value()->readLine();
            parseData(recvdata);
        }
    }
    for(int i=unknownconnection.size()-1;i>=0;--i){
        int roomid;
        while(i>=0&&i<unknownconnection.size()&&unknownconnection[i]->bytesAvailable()>0){
            recvdata=unknownconnection[i]->readLine(0);
            roomid=getRoomID(recvdata);
            if(roomid!=-1){
                if(roomsocketmap.contains(roomid)==false){
                    roomsocketmap.insert(roomid,unknownconnection[i]);
                    unknownconnection.removeAt(i);
                }
                else{
                    unknownconnection[i]->disconnectFromHost();
                    unknownconnection.removeAt(i);
                }
            }
            parseData(recvdata);
        }
    }
}

void NetControllor::processNewConnection(){
    unknownconnection.push_back(mainserver->nextPendingConnection());
    connect(unknownconnection.last(),&QTcpSocket::readyRead,this,&NetControllor::processData);
    connect(unknownconnection.last(),&QTcpSocket::disconnected,unknownconnection.last(),&QTcpSocket::deleteLater);
}
