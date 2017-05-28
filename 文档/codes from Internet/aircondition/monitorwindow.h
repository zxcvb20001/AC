#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QWidget>
#include <QMap>
#include "DataStruct.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>
#include <QDoubleSpinBox>
#include "constant.h"

namespace Ui {
class MonitorWindow;
}

class MonitorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorWindow(const QMap<int,SingleSubMacInfo> *dataptr,QWidget *parent = 0);
    ~MonitorWindow();

public slots:
    void changeFreq(double newval);//change the frequency of updating table

private:
    Ui::MonitorWindow *ui;

    const QMap<int,SingleSubMacInfo> *monitorinfo;
    QTimer *updatetimer;

private slots:
    void updatetable();
};

#endif // MONITORWINDOW_H
