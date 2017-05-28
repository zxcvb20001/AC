#include "mainwindow.h"
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QObject>
#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    bool ok=false;
    int roomid;
    roomid=QInputDialog::getInt(0,"空调","房间号",0,1,9,1,&ok);
    if(ok==false){
        QMessageBox::warning(0,"错误","未指定房间号，无法正常开机！",QMessageBox::Ok);
        exit(0);
    }
    w.setRoomID(roomid);
    w.show();

    return a.exec();
}
