#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>
#include <QDateTime>
#include <time.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTableWidgetItem>
#include <QPropertyAnimation>
#include "connectdb.h"
#include "central.h"
#include "my_server.h"
#include "my_socket.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    my_server *my_tcpserver; //tcp服务器
    QString send_message[20];  //存放要发送的字符串数组
    int getWorkState(); //获取工作状态
    int getTurnOn(); //获取开关机状态
    int getSlaveNumber(); //获取连接从机数
    int getSchedule(); //获取主机的调度算法

private slots:
    void timeslot();
    void listen(); //监听函数
    void DealWithBag(); //处理数据包函数
    void on_start_button_clicked();

    void on_report_button_clicked();

    void on_config_button_clicked();

    void on_save_config_button_clicked();

    void on_search_pushButton_clicked();

    void on_config_close_pushButton_clicked();

    void on_report_table_close_pushButton_clicked();

    void on_calendar_pushButton_clicked();

    void on_clear_db_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    int settemp; //设定温度
    int turnOnState; //是否开机，1为开机，0为未开机
    int workState; //工作状态，0为制冷，1为供暖，2为待机
    int slave_number; //连接的从机数目
    int schedule; //主机对从机的调度算法
    int updateFrequency; //中央空调刷新频率，以秒为单位
    int turn; //时间片轮转
    QList<my_socket*>socket_list;

    //数据库模型
    QSqlTableModel *model;
    QSqlQuery query;

    //用户信息结构体数组
    typedef struct userNode{
        int slave_no;
        QString password;
        int LoginState;
    }userNode;
    userNode userInfo[20];

    //已连接用户的请求链表结构体
    typedef struct slaveNode{
        int slave_no;  //从机号
        int mode; //从机的工作模式
        double startTemp; //起始温度
        double current_temp; //当前室温
        int set_temp; //设定温度
        int windspeed; //风速
        int isRequest; //表示是否有温控需求
        int isWorking; //表示是否正在工作
        QString connect_longtimestamp; //详细连接时间
        QString connect_timestamp; //连接时间
        QTime connect_duration; //连接时长,以秒为单位
        double power_consumption; //用电量
        double cost; //应付金额
        slaveNode* next;

        slaveNode()
        {
            next = NULL;
            QDateTime time_now;
            startTemp = -1;
            connect_timestamp = time_now.currentDateTime().toString("hh:mm:ss");
            connect_longtimestamp = time_now.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            connect_duration = QTime(0,0,0);
            isWorking = 0;
            isRequest = 1;
            mode = cool;
            power_consumption = 0;
            cost = 0;
        }
    }slaveNode;
    slaveNode* head;

    void setWorkState(int); //设定工作状态
    void setTurnOn(int); //设定开关机状态
    void setSlaveNumber(int); //更新连接的从机数目
    void setSchedule(int); //设置主机的调度算法
    void DealWithRequest(QString); //处理从机请求的函数
    void DealWithData(QString); //处理从机发来的数据的函数
    void addSlave(int); //添加从机结点
    void updateSlave(QString); //更新从机状态
    void deleteSlave(int); //删除从机结点
    void UpdateCombobox(); //更新从机队列的combobox
    bool createConnection(); //连接数据库
    void addDataBase(slaveNode*); //增加一条数据库数据
};

#endif // MAINWINDOW_H
