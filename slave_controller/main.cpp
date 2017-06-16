#include "mainwindow.h"
#include <QApplication>
#include "QThread"
#include "global.h"

//10.128.197.89
//10.28.219.9
const QString ipAdress = "10.128.197.89";
QTcpSocket *tcpSocket=new QTcpSocket();
QHostAddress hostAddress = QHostAddress(ipAdress);//QHostAddress::LocalHost;//Ip地址
const int port=6661;//端口
int g_nOutTem=35;//室外温度,这个值根据工作模式而来
int initTem;//缺省温度变量
//Room *roomFor;

QTimer *g_time=new QTimer();//全局定时器，用于定时发送data包（当前温度，风速，设置温度等）
QTimer *g_TemProTime=new QTimer();//全局定时器，用于定时推进房间温度（模拟房间温度变化）

QString roomID;//房间号
QString ID;//身份证号
int g_nUpgradeSpeed=5;//刷新频率，默认为5秒，可以随着中央空调的要求改变而改变

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
