#include<QDebug>
#include<QMessageBox>
#include"my_server.h"
#include"my_socket.h"

my_server::my_server(QWidget * parent)
    :QTcpServer(parent)
{
    //this->socket_list = new QList<my_socket*>;
}
void my_server::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"有新的从机连接！";
    my_socket *m_socket = new my_socket();
    m_socket->tcpSocket->setSocketDescriptor(socketDescriptor);
    socket_list.append(m_socket); //服务器增加一个socket队列
}
