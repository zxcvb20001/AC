#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QPushButton>
#include <QDate>
#include "datacontrollor.h"
#include "netcontrollor.h"
#include "reportwindow.h"
#include "monitorwindow.h"
#include "DataStruct.h"
#include <QMap>
#include <QList>
#include <QTimer>
#include <QPair>
#include <QMultiMap>
#include <QDateTime>
#include <QTimer>
#include <QHostAddress>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QMap<int,SingleSubMacInfo> submacinfo;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onModeClick();
    void subMachineStart(int roomid);
    void subMachineStop(int roomid);
    void subMachineRequestWind(int roomid,int targettemp,int windspeed);
    void subMachineStopWind(int roomid);
    void updateRoomTemp(int roomid,int roomtemp,int windspeed);
    void openCloseCentralMachine();

private:
    Ui::MainWindow *ui;
    QButtonGroup *workModeGroup;
    QTimer *sendwindtimer;

    DataControllor *datacontrollor;
    NetControllor *netcontrollor;
    ReportWindow *reportwindow;
    MonitorWindow *monitorwindow;

    bool isopen;
    bool workmode;
    int currenteentralT;
    QList<int> waitinglist;//restore rooms whitch have sent request and waiting to receive wind
    QList<int> serveinglist;//restore rooms whitch are receiving wind

    void setCentralT(int newval);
    void openCentralMachine();
    void closeCentralMachine();

private slots:
    void increaseCenralT();
    void decreaseCenralT();
    void setMonitorVisible();//show monitor table
    void setReportVisible();//show report table
    void sendWindToSubMachine();
};

#endif // MAINWINDOW_H
