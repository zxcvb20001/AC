#ifndef MY_SOCKET_H
#define MY_SOCKET_H

#include <QQueue>
#include <QWidget>
#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>

class my_socket: public QObject
{
    Q_OBJECT
public:
    explicit my_socket();
    ~my_socket();
    QTcpSocket *tcpSocket;
    void sendMessage(QString); //发送数据
    QQueue<QString>DataBagList; //数据包队列

private:
    //TCP连接
    QStringList message_List;
    QString rec_message;  //存放从服务器接收到的字符串
    quint16 blockSize; //存放文件的大小信息

//signals:
//    force();

private slots:
    //TCP槽函数
    void readMessage();  //接收数据
    void displayError(QAbstractSocket::SocketError);  //显示错误
    void disconnectInfo(); //连接断开
};

#endif // MY_SOCKET_H
