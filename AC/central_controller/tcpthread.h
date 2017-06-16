#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include<QThread>
#include<QTcpSocket>
class my_socket;

class tcpThread :public QThread
{
private:
    qintptr ptr;
    my_socket *socket;

public:
    tcpThread(QWidget *parent,qintptr p);
    my_socket* getSocket(); //获取socket

protected:
    virtual void run();

#endif // TCPTHREAD_H
