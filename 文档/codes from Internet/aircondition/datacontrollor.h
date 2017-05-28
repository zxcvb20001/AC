#ifndef DATACONTROLLOR_H
#define DATACONTROLLOR_H

#include <QObject>
#include <QSqlDatabase>
#include "constant.h"
#include "DataStruct.h"
#include <QVector>
#include <QSqlQuery>
#include <QDateTime>

class DataControllor : public QObject
{
    Q_OBJECT
public:
    explicit DataControllor(QObject *parent = 0);
    //add data to database
    void addAdjust(const int newroomid,const SingleAdjustData newdata);
    void addOpenClose(const int newroomid, const QDateTime newdata);

public slots:
    void getAllData(QVector<SingleReportData> &alldata);
    void clearAllData();

private:
    QSqlDatabase db;
};

#endif // DATACONTROLLOR_H
