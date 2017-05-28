#ifndef NETCONTROLLER_H
#define NETCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QtNetwork/QTcpSocket>
#include <QChar>
#include "constants.h"
#include <QDataStream>
#include <QHostAddress>
#include <QByteArray>

class netController : public QObject
{
    Q_OBJECT
signals:
    void recvOpenAck(bool mode);
    void recvWind(bool mode);
    void compelCLose();
public:
    explicit netController(QObject *parent = 0);
    void sendWindRequest(int roomid,int targettemp,int windspeed);
    void sendRoomTemp(int roomid,int roomtemp,int windspeed);
    void sendCloseInfo(int roomid);
    bool preSendOpenInfo(int roomid);//return true when connected,false otherwise
public slots:
    void sendOpenInfo();
private:
    QTcpSocket *tsock;
    int pendingroomid;//used to restore roomid temporaily

private slots:
    void processRecvData();
    void lostconnect();
};

#endif // NETCONTROLLER_H
