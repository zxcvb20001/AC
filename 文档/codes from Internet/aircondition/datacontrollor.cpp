#include "datacontrollor.h"
#include <QDebug>

DataControllor::DataControllor(QObject *parent) :
    QObject(parent)
{
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("linkdata.db");//database name
    db.open();
}

void DataControllor::addAdjust(const int newroomid, const SingleAdjustData newdata){
    qDebug()<<"add adjust data";
    QSqlQuery query;
    query.prepare("SELECT roomid FROM room where roomid=?");
    query.bindValue(0,newroomid);
    query.exec();
    if(query.next()==false){
        query.prepare("INSERT INTO room(roomid) VALUES(?)");
        query.bindValue(0,newroomid);
        query.exec();
    }
    query.prepare("INSERT INTO adjust(roomid,mode,speed,starttemp,stoptemp,starttime,stoptime,fee) VALUES(?,?,?,?,?,?,?,?)");
    query.bindValue(0,newroomid);
    query.bindValue(1,newdata.mode);
    query.bindValue(2,newdata.speed);
    query.bindValue(3,newdata.starttemp);
    query.bindValue(4,newdata.stoptemp);
    query.bindValue(5,newdata.starttime);
    query.bindValue(6,newdata.stoptime);
    query.bindValue(7,newdata.fee);
    query.exec();
}

void DataControllor::addOpenClose(const int newroomid,const QDateTime newdata){
    QSqlQuery query;
    query.prepare("SELECT roomid FROM room where roomid=?");
    query.bindValue(0,newroomid);
    query.exec();
    if(query.next()==false){
        query.prepare("INSERT INTO room(roomid) VALUES(?)");
        query.bindValue(0,newroomid);
        query.exec();
    }
    query.prepare("INSERT INTO openclose(roomid,time) VALUES(?,?)");
    query.bindValue(0,newroomid);
    query.bindValue(1,newdata);
    query.exec();
}

void DataControllor::getAllData(QVector<SingleReportData> &alldata){
    while (alldata.isEmpty()==false) {
        alldata.pop_back();
    }
    QSqlQuery query("SELECT roomid FROM room");
    while(query.next()){
        alldata.push_back(SingleReportData(query.value(0).toInt()));
    }
    for(QVector<SingleReportData>::Iterator i=alldata.begin();i!=alldata.end();++i){
        query.prepare("SELECT time from openclose where roomid=?");
        query.bindValue(0,i->roomid);
        query.exec();
        while(query.next()){
            i->openclosedata.push_back(query.value(0).toDateTime());
        }
        query.prepare("SELECT mode,speed,starttemp,stoptemp,starttime,stoptime,fee from adjust where roomid=?");
        query.bindValue(0,i->roomid);
        query.exec();
        while(query.next()){
            SingleAdjustData adjustdata(query.value(0).toBool(),query.value(1).toInt(),query.value(2).toInt(),query.value(3).toInt(),query.value(4).toDateTime(),query.value(5).toDateTime(),query.value(6).toDouble());
            i->adjustdata.push_back(adjustdata);
        }
    }
}

void DataControllor::clearAllData(){
    QSqlQuery query;
    query.exec("delete from adjust");
    query.exec("delete from openclose");
    query.exec("delete from room");
}
