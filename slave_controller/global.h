#ifndef GLOBAL_H
#define GLOBAL_H
#include<QTcpSocket>
#include<QHostAddress>
#include<QTimer>
#include <QVector>


#define ONLINE 0
#define OFFLINE 1
#define ONWORK 0
#define OFFWORK 1


extern QTcpSocket *tcpSocket;
extern QHostAddress hostAddress;//Ip地址
extern const int port;//端口
extern int g_nOutTem;//室外温度,这个值根据工作模式而来
extern QString roomID;//房间号
extern QString ID;//身份证号
extern int g_nUpgradeSpeed;//刷新频率
extern QTimer *g_time;
extern QTimer *g_TemProTime;//全局定时器，用于定时推进房间温度（模拟房间温度变化）
extern int initTem;//缺省温度变量


#endif // GLOBAL_H
