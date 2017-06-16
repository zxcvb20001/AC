#include "my_socket.h"

my_socket::my_socket()
{
    //槽函数
    //TCP连接socket套接字
    tcpSocket = new QTcpSocket(this);
    this->connect(this->tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage())); //服务器接收客户端的消息
    this->connect(this->tcpSocket,SIGNAL(disconnected()), this, SLOT(disconnectInfo()));
    //emit(force());

}

my_socket::~my_socket()
{
    ;
}

void my_socket::readMessage()//读取数据
{
//    QDataStream in(tcpSocket);
//    in.setVersion (QDataStream::Qt_5_8);
//    in >> rec_message;
//    qDebug() << "socket" << m_tcpsocket << endl;
    QByteArray tt = this->tcpSocket->readAll();
    this->rec_message = QVariant(tt).toString();
    qDebug() << "ser message" << rec_message;

    QStringList list = rec_message.split(";");//切割数据
    int len = list.size();
    for(int i = 0; i < len; i++)
       DataBagList.enqueue(list[i]);
    rec_message.clear();
}

void my_socket::disconnectInfo()
{
    tcpSocket->close();
    qDebug() << "一台从控机已经断开连接";
}

void my_socket::sendMessage(QString send)//发送数据
{
//    //qDebug() << "send socket" << m_tcpsocket << endl;
//    //用于暂存我们要发送的数据
//    QByteArray block;

//    //使用数据流写入数据
//    QDataStream out(&block,QIODevice::WriteOnly);

//    //设置数据流的版本，客户端和服务器端使用的版本要相同
//    out.setVersion(QDataStream::Qt_5_8);

//    out<<(quint16) 0;
//    out << send;
//    out.device()->seek(0);
//    out<<(quint16)(block.size()-sizeof(quint16));
//    this->tcpSocket->write(block);
     tcpSocket->write(send.toStdString().c_str(),strlen(send.toStdString().c_str()));

    //发送数据成功后，显示提示
    qDebug() << "send server message successfully!!!" << endl;
    qDebug() << send << endl;
}


void my_socket::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString(); //输出错误信息
}


