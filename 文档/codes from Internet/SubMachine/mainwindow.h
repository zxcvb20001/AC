#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <ctime>
#include <Qtimer>
#include <QMessageBox>
#include "netcontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setRoomID(int newval);
public slots:
    void openFailed();
    void onWindSpeedClick();
    void OpenMachine(bool mode);
    void CloseMachine();
    void ReceiveWind(bool mode);
    void handleOpenClose();
    void increaseTargetTemp();
    void decreaseTargetTemp();

private:
    netController netcon;
    Ui::MainWindow *ui;
    QButtonGroup *windSpeedGroup;
    QTimer *regresstemptimer;
    QTimer *setdelaytimer;
    QTimer *judgetimer;

    bool isopen;
    bool currentmode;//0:coldmode,1:warm mode
    int roomid;
    int roomtemp;
    int targettemp,temp_targettemp;
    int currentwindspeed;//0:low,1:mid,2:high
    double usedfee;
    double usedenergy;

    void changeWindSpeed(int newspeed);
    void generateRoomTemp();
private slots:
    void regressTemp();
    void changeTargetTemp();
    void judgeTemp();
};

#endif // MAINWINDOW_H
