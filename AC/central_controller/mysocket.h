#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QQueue>
#include <QObject>
#include <QHostAddress>
#include <QMainWindow>


class mySocket:public QWidget
{
    Q_OBJECT
public:
    explicit mySocket(QWidget *parent);
    ~mySocket();
    QString send_message;  //存放要发送的字符串
    void sendMessage(); //发送数据
    QQueue<QString>DataBagList; //数据包队列

private:
    //TCP连接
    QTcpSocket *m_tcpsocket;
    QTcpServer *m_tcpserver;
    //QList socket_list= new QList<QTcpSocket*>;
    QStringList message_List;
    //QTcpSocket *tcpSocket;
    QString rec_message;  //存放从服务器接收到的字符串
    quint16 blockSize; //存放文件的大小信息

    void listen();

private slots:
    //TCP槽函数
    void newConnect(); //连接服务器
    void readMessage();  //接收数据
    void displayError(QAbstractSocket::SocketError);  //显示错误
};

#endif // MYSOCKET_H
