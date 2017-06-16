#ifndef ROOM_H
#define ROOM_H

#include <QDialog>
#include <QDebug>
#include <QPropertyAnimation>
#include <QMessageBox>

namespace Ui {
class Room;
}

class Room : public QDialog
{
    Q_OBJECT

public:
    explicit Room(QWidget *parent = 0);
    ~Room();
    int _GetCurrentTem();
    int _GetSettedTem();
    int _GetWindSpeed();
    int _GetUnWindSpeed();
    double _GetMoneyUsing();
    double _GetElecUsing();
    int _GetWorkStyle();
    int _GetUnSettedTem();


    void _SetCurrentTem(int nCurrentTem_temp);
    void _SetWindSpeed(int nWindSpeed_temp);
    void _SetUnWindSpeed(int nUnWindSpeed_temp);
    void _SetMoneyUsing(double fMoneyUsing_temp);
    void _SetElecUsing(double fElecUsing);
    void _SetWorkStyle(int nWorkStyle_temp);
    void _SetSettedTem(int nSettedTem_temp);
    void _SetUnSettedTem(int nUnSettedTem_temp);
    void arrange();
private slots:
    void on_up_btn_clicked();

    void on_down_btn_clicked();

    void on_ok_btn_clicked();

    void on_high_btn_clicked();

    void on_middle_btn_clicked();

    void on_low_btn_clicked();

    void on_stop_btn_clicked();

    void on_over_btn_clicked();

    void on_report_clicked();

    void on_loginBtn_2_clicked();

    void on_windspeed_button_clicked();

    void on_model_button_clicked();

    void on_close_clicked();

    void on_master_button_clicked();

private:
    Ui::Room *ui;
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
};


#endif // ROOM_H
