#ifndef NETCONTROLLOR_H
#define NETCONTROLLOR_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QMap>
#include <QList>
#include "constant.h"
#include <QTimer>

class NetControllor : public QObject
{
    Q_OBJECT
signals:
    void subMachineStart(int roomid);
    void subMachineStop(int roomid);
    void subMachineRequestWind(int roomid,int targettemp,int windspeed);
    void subMachineStopWind(int roomid);
    void updateRoomTemp(int roomid,int roomtemp,int windspeed);
public:
    explicit NetControllor(QObject *parent = 0);

    void startserver();
    void stopserver();
    void sendMode(int roomid,bool currentmode);
    void sendWind(int roomid,bool windtype);
    void sendCompelStop(int roomid);

private:
    QTcpServer *mainserver;
    QMap<int,QTcpSocket*> roomsocketmap;
    QList<QTcpSocket*> unknownconnection;//restore connections whitchs haven't sent roomid

    void parseData(QString &recvdata);
    int getRoomID(QString &recvdata);
private slots:
    void processData();
    void processNewConnection();
};

#endif // NETCONTROLLOR_H
