#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void WaitSec(int s);

    ~MainWindow();
    /*
    int _GetCurrentTem();
    int _GetSettedTem();
    int _GetWindSpeed();
    int _GetUnWindSpeed();
    double _GetMoneyUsing();
    double _GetElecUsing();
    int _GetWorkStyle();
    int _GetUnSettedTem();
*/

    void _SetCurrentTem(int nCurrentTem_temp);
    void _SetWindSpeed(int nWindSpeed_temp);
    void _SetUnWindSpeed(int nUnWindSpeed_temp);
    void _SetMoneyUsing(double fMoneyUsing_temp);
    void _SetElecUsing(double fElecUsing);
    void _SetWorkStyle(int nWorkStyle_temp);
    void _SetSettedTem(int nSettedTem_temp);
    void _SetUnSettedTem(int nUnSettedTem_temp);
    void sendMessage(QString s);

    void arrange();

private slots:

    void TemPropel();//温度推进函数
    void readMessage();
    void sendMessage1();


    void on_report_clicked();

    void on_loginBtn_clicked();

    void on_windspeed_button_clicked();

    void on_model_button_clicked();

    void on_close_clicked();

    void on_master_button_clicked();

    void on_up_temp_button_clicked();

    void on_down_temp_button_clicked();

    void on_speed_button_clicked();

    void on_logout_clicked();

    void on_warm_button_clicked();

    void on_cool_button_clicked();

private:
    //Room *roomFor=new Room();//创建一个房间类变量
    bool flag1;
    bool flag2;
    bool flag3;
    Ui::MainWindow *ui;

    int m_nCurrentTem;//现在的温度
    int m_nSettedTem;//温度设置,这个温度设置为确定连接有效后的温度
    int m_nUnSettedTem;//这个设置温度为显示在设置面板上的温度，未确认有效
    int m_nWindSpeed;//风速
    int m_nUnWindSpeed;//为确认有效的风速
    double m_fElecUsing;//电力用量
    double m_fMoneyUsing;//消耗金额
    int m_nWorkStyle;//工作模式
    int loginState;
    int switchState;
    int counter;
    int dataNum;
    QVector<double> tempData;
    //QTimer *time=new QTimer(this);
};



#endif // MAINWINDOW_H
