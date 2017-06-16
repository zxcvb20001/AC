#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTcpSocket>
#include <QMessageBox>
#include <QPushButton>
#include <QTime>
#include <cmath>
#include <QPropertyAnimation>
#include <QTimer>
#include <QHostAddress>
#include <QDebug>
#include <QQueue>
#include <QDesktopWidget>
#include  <QVector>
#include "myGlobal.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int getSettemp(); //获得设定温度
    int getLoginState(); //获得登录状态
    double getCurrentTemp(); //获得当前室温
    int getWindSpeed(); //获得工作风速
    int getWorkState(); //获得当前工作状态
    int getSlaveNo(); //获得从机编号
    int getTurnOn(); //获得开关机状态
    int getWorkMode(); //获得工作模式
    double getConsumpt(); //获得用量
    double getBill(); //获得金额

private slots:

    //TCP槽函数
    void newConnect();//连接请求函数
    void sendMessage();//发送信息
    void readMessage();//读取信息
    void displayError(QAbstractSocket::SocketError);  //显示错误
    void DealWithBag(); //处理数据包函数

    void timeslot(); //界面刷新槽函数

    void on_master_button_clicked();

    void on_cool_button_clicked();

    void on_warm_button_clicked();

    void on_model_button_clicked();

    void on_speed_button_clicked();

    void on_windspeed_button_clicked();

    void on_up_temp_button_clicked();

    void on_down_temp_button_clicked();

    void on_logout_clicked();

    void on_report_clicked();

    void on_loginBtn_clicked();

    void on_close_clicked();

    void on_again_clicked();

private:
    Ui::MainWindow *ui;
    int turnOn; //开关机状态
    int settemp; //设定温度
    int loginState;  //登录状态，1为已登录，0为未登录
    int workMode; //工作模式，1为制冷，2为供暖
    int workState; //工作状态，1为正在工作，2为空闲
    int windSpeed; //风速，1为低速风，2为中速风，3为高速风
    int sendCnt; //发送数据计数
    int slotCnt; //时间槽函数计数
    int slave_no; //从机编号
    int sendDataFrequency; //设置发送数据的频率，初始为1秒/次
    int isRemindMode; //是否已经提醒模式更改
    double currentTemp; //实时室温
    double consumption; //用量
    double bill; //金额
    int counter;
    int dataNum;
    QVector<double> tempData;
    QQueue<QString>DataBagList; //数据包队列

    void setTemp(int);  //设定目标温度
    void setSlaveNo(int); //设置从机编号
    void setLoginState(int); //修改登录状态
    void setWorkState(int); //设定工作状态
    void setWorkMode(int);  //设定工作模式
    void setWindSpeed(int); //设定工作风速
    void setTurnOn(int); //设置开关机状态
    void setCurrentTemp(double); //初始化室温
    void DealWithInstruction(QString); //处理主机发来的instruction包
    void DealWithInformation(QString); //处理主机发来的information包
    // 安排各部件的位置或者是否隐藏
    void arrange();

    //TCP连接
    QTcpSocket *tcpsocket;
    QString send_message;  //存放从准备发送的字符串
    QString rec_message;  //存放接收到的字符串
    QString message;  //存放从服务器接收到的字符串
    quint16 blockSize; //存放文件的大小信息

};

#endif // MAINWINDOW_H
