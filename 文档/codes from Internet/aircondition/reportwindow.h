#ifndef REPORTWINDOW_H
#define REPORTWINDOW_H

#include <QWidget>
#include <QButtonGroup>
#include <QRadioButton>
#include "datacontrollor.h"
#include <QVector>
#include "DataStruct.h"
#include <QDateEdit>
#include <QDate>
#include <QDateTime>
#include <QCalendarWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include "adjustdialog.h"
#include <QString>

namespace Ui {
class ReportWindow;
}

class ReportWindow : public QWidget
{
    Q_OBJECT

signals:
    void getReportData(QVector<SingleReportData> &rdata);//block until return
    void clearAllData();
public:
    explicit ReportWindow(QWidget *parent = 0);
    ~ReportWindow();

public slots:
    void fillTable();//fillint table with current data
    void updateData();
    void updateAll();
    void processClick(int row,int col);
    void clearTableAndData();//clear content in table and data in database
    void changeRoomNum();

private slots:
    void onReportFormatclick();

private:
    Ui::ReportWindow *ui;
    QButtonGroup *reportFromatGroup;

    QVector<SingleReportData> reportdata;
    int currentreportmode;//0:day,1:week,2:month
    int currentroomnum;
};

#endif // REPORTWINDOW_H
