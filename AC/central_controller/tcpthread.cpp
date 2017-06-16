#include"tcpthread.h"
#include"my_socket.h"

tcpThread::tcpThread(QWidget *parent,qintptr p):QThread(parent)
{
    //qDebug()<<"QThread构造函数依然在旧线程";
    this->ptr=p;
}

my_socket tcpThread::getSocket()
{
    return this->socket;
}

void tcpThread::run()
{
    qDebug()<<"开始新线程";
    socket = new my_socket(0,this->ptr);
    while(true)
        this->exec();
    //socket->waitForBytesWritten();
}
