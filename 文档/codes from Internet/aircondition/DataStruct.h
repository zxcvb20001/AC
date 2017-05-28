#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QDateTime>
#include <QVector>

class SingleSubMacInfo;
class SingleReportData;
class SingleAdjustData;

class SingleSubMacInfo{
public:
    int currentT;
    int targetT;
    int speed;
    double useE;
    double useF;
    //retrore adjust start info
    QDateTime adjuststarttime;
    int adjuststarttemp;

    SingleSubMacInfo(){
        currentT=-1;
        targetT=-1;
        speed=-1;
        useE=0;
        useF=0;
    }

    SingleSubMacInfo(int ct,int tt){
        currentT=ct;
        targetT=tt;
        speed=-1;
        useE=0;
        useF=0;
    }
};

class SingleReportData{
public:
    SingleReportData(){}
    SingleReportData(int val){
        roomid=val;
    }

    int roomid;
    QVector<QDateTime> openclosedata;
    QVector<SingleAdjustData> adjustdata;
};

class SingleAdjustData{
public:
    SingleAdjustData(){}
    SingleAdjustData(bool modeval,int speedval,int starttempval,int stoptempval,QDateTime starttimeval,QDateTime stoptimeval,double feeval){
        mode=modeval;
        speed=speedval;
        starttemp=starttempval;
        stoptemp=stoptempval;
        starttime=starttimeval;
        stoptime=stoptimeval;
        fee=feeval;
        shown=false;
    }

    bool mode;
    int speed;
    int starttemp;
    int stoptemp;
    QDateTime starttime;
    QDateTime stoptime;
    double fee;
    //used in display report table
    bool shown;
};

#endif // DATASTRUCT_H
