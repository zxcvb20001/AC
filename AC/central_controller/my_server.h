#ifndef MY_SERVER_H
#define MY_SERVER_H

#include<QList>
#include<QWidget>
#include<QObject>
#include<QTcpServer>
#include"my_socket.h"

class my_server :public QTcpServer
{
    Q_OBJECT
public:
    my_server(QWidget * parent);
    QList<my_socket*>socket_list; //服务器的socket队列

protected:
    virtual void incomingConnection(qintptr); //服务器新连接


};

#endif // MY_SERVER_H
