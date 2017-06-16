#include "mainwindow.h"
#include "mysocket.h"

mySocket::mySocket(QWidget *parent)
{
    //槽函数
    //TCP连接socket套接字
    m_tcpserver = new QTcpServer(this);
    listen();
    connect(m_tcpserver,SIGNAL(newConnection()),this,SLOT(newConnect()));//一旦连接就侦听
}

void mySocket::listen()//侦听
{
    m_tcpserver->listen(QHostAddress::Any,6666);//监听的端口号
    if(!m_tcpserver->listen(QHostAddress::Any,6666))
    {  //**本地主机的6666端口，如果出错就输出错误信息，并关闭
        qDebug() << m_tcpserver->errorString();
        close();
    }
}

void mySocket::newConnect()//连接
{
    m_tcpsocket = m_tcpserver->nextPendingConnection();//设置监听
    connect(m_tcpsocket,SIGNAL(readyRead()),this,SLOT(readMessage())); //服务器接收客户端的消息
    connect(m_tcpsocket,SIGNAL(disconnected()),m_tcpsocket,SLOT(deleteLater()));
}

void mySocket::readMessage()//读取数据
{
    QDataStream in(m_tcpsocket);
    in.setVersion (QDataStream::Qt_5_8);
    in >> rec_message;
//    qDebug() << "socket" << m_tcpsocket << endl;
    qDebug() << "ser message" << rec_message;
    DataBagList.enqueue(rec_message);
    rec_message.clear();
}

void mySocket::sendMessage()//发送数据
{
    //qDebug() << "send socket" << m_tcpsocket << endl;
    //用于暂存我们要发送的数据
    QByteArray block;

    //使用数据流写入数据
    QDataStream out(&block,QIODevice::WriteOnly);

    //设置数据流的版本，客户端和服务器端使用的版本要相同
    out.setVersion(QDataStream::Qt_5_8);

    out<<(quint16) 0;
    out << send_message;
    out.device()->seek(0);
    out<<(quint16)(block.size()-sizeof(quint16));

    connect(m_tcpsocket,SIGNAL(disconnected()), m_tcpsocket, SLOT(deleteLater()));
    m_tcpsocket->write(block);
    //m_tcpsocket->disconnectFromHost();

    //发送数据成功后，显示提示
//    qDebug() << "send server message successfully!!!" << endl;
//    qDebug() << send_message << endl;
}

void mySocket::displayError(QAbstractSocket::SocketError)
{
    qDebug() << m_tcpsocket->errorString(); //输出错误信息
}
