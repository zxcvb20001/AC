#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"global.h"
//#include"sendthread.h"
#include<QTime>
#include<QObject>
#include<QThread>
#include<QWaitCondition>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    flag1=false;//用于判断是否是第一次连接
    flag2=false;//用于判断是否有送风
    flag3=false;//用于判断是否为自动停止送风的情况


    //this->move ((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);
    ui->setupUi(this);

    this->counter = 0;
    this->dataNum = 0;
    this->tempData.clear();
    setFixedSize(485, 550);
    this->m_nCurrentTem = g_nOutTem;
    arrange();
    g_TemProTime->start(3000);//温度推进定时器开始推进
    this->loginState = OFFLINE;
    this->switchState = OFFWORK;

    /* 初始值 */
    //m_nUnSettedTem = 22;

    connect(g_time,SIGNAL(timeout()),this,SLOT(sendMessage1()));//发送信息定时器，信息包括房间的状态信息
    connect(g_TemProTime,SIGNAL(timeout()),this,SLOT(TemPropel()));//温度推进按钮
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));//一直为等待接受信息的状态

    //connect(SendFromCient,SIGNAL(sendMessageSignal(s)),this,SLOT(sendMessage(s)));
}


void MainWindow::readMessage(){
    QByteArray qba=tcpSocket->readAll();
    qDebug()<<qba;
    QString ss=QVariant(qba).toString();

    QString s;
    if(ss.indexOf('i')!=-1){//找到instruction开头或者information开头的下标
        s=ss.mid(ss.indexOf('i'));//s 为从下标出截取
        qDebug()<<"s 为: "<<s;
    }
    int indexFF=0;
    int len=s.length();//得到s的字符串长度，用于判断是否到最后了
    qDebug()<<"s 的长度为: "<<len;

    while(indexFF!=len){

        QString str1="",str2="",str3="",str4="",str5="",str6="";//共有6个数据域
        //str1为信息包头信息
        while(s[indexFF]!='#'&&indexFF!=len){
            str1+=s[indexFF];
            indexFF++;
        }
        if(indexFF==len)//这个时候信息包不完全,直接跳出
            break;
        indexFF++;
        if(indexFF==len)//这个时候信息包不完全,直接跳出
            break;
        //str2为操作类型
        str2+=s[indexFF];
        indexFF+=2;//此时指向从控机编号的第一位
        if(indexFF==len)//这个时候信息包不完全,直接跳出
            break;
        //str3存储的是从控机的编号
        while(s[indexFF]!='#'&&indexFF!=len){
            str3+=s[indexFF];
            indexFF++;
        }
        if(indexFF==len)//这个时候信息包不完全,直接跳出
            break;
        indexFF++;
        if(indexFF==len)//这个时候信息包不完全,直接跳出
            break;
        //str4存储的是有效位或者数据域1
        while(s[indexFF]!='#'&&indexFF!=len){
            str4+=s[indexFF];
            indexFF++;
        }
        if(indexFF==len)//这个时候信息包不完全,直接跳出
            break;
        indexFF++;
        if(indexFF==len)//这个时候信息包不完全,直接跳出
            break;
        //str5为数据域1或者数据域2
        while(s[indexFF]!='#'&&indexFF!=len){
            str5+=s[indexFF];
            indexFF++;
        }
        if(indexFF==len)//这个时候信息包不完全,直接跳出
            break;
        indexFF++;
        if(indexFF==len)//这个时候信息包不完全,直接跳出
            break;
        //str6存储的是数据域2或者数据域3
        while(s[indexFF]!='#'&&indexFF!=len){
            str6+=s[indexFF];
            indexFF++;
        }
        indexFF=indexFF+2;

        if(str1=="instruction"){
            if(str2=='1'){//连接认证响应
                qDebug()<<"连接认证开始响应";
                if(str4=='0'){//连接无效
                    qDebug()<<"连接认证不成功";

                }
                else if(str4=='1'){//连接有效
                    qDebug()<<"连接认证成功";
                    //this->show();
                    ui->login_group->setVisible(false);

                    if(str5=='0'){//表示工作模式为冷风
                        this->_SetWorkStyle(0);//设置工作模式为冷风
                        g_nOutTem=35;//当制冷时，我们默认室外温度为35℃
                        qDebug()<<"工作模式为冷风，设置室外温度为35℃";

                        ui->current_choice->setText("当前模式：制冷");
                    }
                    else if(str5=='1'){//表示工作模式为暖风
                        this->_SetWorkStyle(1);//设置工作模式为暖风
                        g_nOutTem=15;//当制冷时，我们默认室外温度为15℃
                        qDebug()<<"工作模式为暖风，设置室外温度为15℃";

                        ui->current_choice->setText("当前模式：制热");
                    }
                    //说明是第一次连接，这个时候把当前的室内温度设置为室外温度,以后的每次当前温度从this中获得，
                    //以便于TemPropel推进温度
                    if(flag1==false){//如果是第一次连接则设置房间当前的温度为室外温度，否则他自己会有显示温度
                        qDebug()<<"这是第一次连接，设置室内温度为室外温度";
                        this->_SetCurrentTem(g_nOutTem);
                        //SendFromClient->_SetRoom(this);//房间类进行传参,第一次进行设置即可
                        //CTemPropel->_SetRoom(this);//房间类进行传参，飞一次进行设置即可
                        flag1=true;
                    }
                    initTem=str6.toInt();//将温度的sting型转换为int型

                    this->_SetSettedTem(initTem);//设置工作缺省温度为设置温度
                    this->_SetUnSettedTem(initTem);//设置工作缺省温度为未确认温度，用于在显示器上显示
                    this->_SetWindSpeed(0);//默认缺省风速为低风,我们这里是从控机自己设置的

                    qDebug()<<"定时发送从控机的温度，风速等的发送线程开始运行";
                    //SendFromCient->start();
                    //g_time->start(g_nUpgradeSpeed*1000);//定时器开始
                    //CTemPropel->open();//温度推进线程显示的是认证已经连接的情况
                    qDebug()<<"温度推进线程现在开始运行";
                    //g_TemProTime->start(3000);//温度推进定时器开始推进
                    //CTemPropel->start();//温度推进线程开始工作
                }
                else if(str4=='2'){//中央空调自己调整工作模式

                    if(str5=='0'){//表示工作模式为冷风
                        this->_SetWorkStyle(0);

                        ui->current_choice->setText("当前模式：制冷");
                    }
                    else if(str5=='1'){//表示工作模式为暖风
                        this->_SetWorkStyle(1);

                        ui->current_choice->setText("当前模式：制热");
                    }

                    initTem=str6.toInt();//将温度的sting型转换为int型

                    this->_SetSettedTem(initTem);//设置工作缺省温度为设置温度
                    this->_SetUnSettedTem(initTem);//设置工作缺省温度为未确认温度，用于在显示器上显示

                }


            }
            else if(str2=='2'){//温度设置响应
                qDebug()<<"温度设置开始响应";
                if(str4=='0'){//温度响应无效
                    qDebug()<<"温度响应无效";
                }
                else if(str4=='1'){//温度响应有效
                    qDebug()<<"温度响应有效";
                    //用未确认有效的温度更新设置温度，用于温度推进函数的推进
                    this->_SetSettedTem(m_nUnSettedTem);
                }
            }
            else if(str2=='3'){//风速设置响应
                qDebug()<<"风速设置开始响应";
                if(str4=='0'){//如果有效位为0
                    qDebug()<<"风速响应无效";
                }
                else if(str4=='1'){//如果有效位为1
                    qDebug()<<"风速设置有效";
                    this->_SetWindSpeed(m_nUnWindSpeed);//用未确认的风速更新风速
                }
            }
            else if(str2=='4'){//刷新频率更新请求
                qDebug()<<"刷频率更新开始响应";
                g_nUpgradeSpeed=str5.toInt();//将刷新频率值更新
                qDebug()<<"刷新频率为: "<<g_nUpgradeSpeed;
                g_time->start(g_nUpgradeSpeed*1000);//定时器根据刷新频率发送给主机
            }
        }
        else if(str1=="information"){
            qDebug()<<"现在开始数据域(用量和金额)的接收";
            this->_SetElecUsing(str4.toDouble());//更新用量
            this->_SetMoneyUsing(str5.toDouble());//更新所用金额
            qDebug()<<"得到用量为:"<<m_fElecUsing;
            qDebug()<<"得到所用金额为:"<<m_fMoneyUsing;

        }
        indexFF=s.indexOf('i',indexFF);
        if(indexFF!=-1){
            s=s.mid(indexFF);
            qDebug()<<"s 为： "<<s;
            indexFF=0;
            len=s.length();//重置len
            qDebug()<<"len 为： "<<len;
        }
        else break;//当信息包无头的时候，直接退出
    }
}

void MainWindow::sendMessage(QString s){//这是一个槽函数,用于发送刷新频率，温度，风速，现在温度等信息
    qDebug()<<"已经发送信息"<<s<<"完毕";
    int k=tcpSocket->write(s.toStdString().c_str(),strlen(s.toStdString().c_str()));
    qDebug()<<"发送返回值为: "<<k;
}

void MainWindow::sendMessage1(){//这是一个槽函数,用于发送刷新频率，温度，风速，现在温度等信息
    //qDebug()<<"已经发送信息"<<s<<"完毕";
    qDebug()<<"根据刷新频率发送温度，风速，设置温度等要求正在准备......";
    //sleep(g_nUpgradeSpeed);//睡眠 刷新频率 的时间
    QString s="";
    s=s+"data#1#"+roomID+'#';
    QString CurrentTem_temp=QString::number(m_nCurrentTem,10);//把当前温度转换为QStirng型
    s=s+CurrentTem_temp+'#';
    QString SettedTem_temp=QString::number(m_nSettedTem,10);//把目标温度转换为QString型
    s=s+SettedTem_temp+'#';
    QString WorkStyle_temp=QString::number(m_nWorkStyle,10);
    s=s+WorkStyle_temp+"#;";
    qDebug()<<"发送的数据包为："<<s;
    int k=tcpSocket->write(s.toStdString().c_str(),strlen(s.toStdString().c_str()));
    qDebug()<<"发送返回值为: "<<k;
}


void MainWindow::TemPropel(){
    if(m_nWindSpeed==3){//停风的情况
        flag2=0;
    }
    else flag2=1;//送风的情况

    if(!flag2){//不送风的情况
        qDebug()<<"不送风";
        if(flag3==false){//手动停止送风的情况
            if(g_nOutTem > m_nCurrentTem){//当室外温度大于室内温度时，一般为夏季的时候
                qDebug()<<"室外温度大于室内温度，室内温度正在升温......";

                int CurrentTem_temp=m_nCurrentTem+1;
                this->_SetCurrentTem(CurrentTem_temp);

                WaitSec(10);
            }
            else if(g_nOutTem < m_nCurrentTem){//当室外温度小于室内温度时
                qDebug()<<"室外温度小于室内温度，室内温度正在降温......";

                int CurrentTem_temp=m_nCurrentTem-1;
                this->_SetCurrentTem(CurrentTem_temp);

                WaitSec(10);
            }
        }
        else if(flag3==true){//自动停止送风的情况
            if(g_nOutTem > m_nCurrentTem){//当室外温度大于室内温度时，一般为夏季的时候
                qDebug()<<"室外温度大于室内温度，室内温度正在升温......";

                int CurrentTem_temp=m_nCurrentTem+1;
                this->_SetCurrentTem(CurrentTem_temp);

                WaitSec(10);
            }
            else if(g_nOutTem < m_nCurrentTem){//当室外温度小于室内温度时
                qDebug()<<"室外温度小于室内温度，室内温度正在降温......";

                int CurrentTem_temp=m_nCurrentTem-1;
                this->_SetCurrentTem(CurrentTem_temp);

                WaitSec(10);
            }
            //室内温度上升一度或者下降一度后,再次要求中央空调送风
            qDebug()<<"温度和目标温度已经相差一度，从控机再次工作，发送送风请求";
            flag3=false;//自动送风标志关闭
            QString s="";//默认送低速风
            this->_SetUnWindSpeed(0);//再次设置未确认风速为0（低风）
            s=s+"request"+"#3#"+roomID+"#0#"+"*#*#;";
            sendMessage(s);
            WaitSec(3);
        }
    }
    else{//送风的情况
        qDebug()<<"送风";
        if(m_nSettedTem > m_nCurrentTem){//设置的温度大于当前的温度
            qDebug()<<"设置温度大于室内温度，室内温度正在升温......";
            if(m_nWindSpeed==0){

                int CurrentTem_temp=m_nCurrentTem+1;
                this->_SetCurrentTem(CurrentTem_temp);

                WaitSec(8);
            }
            else if(m_nWindSpeed==1){

                int CurrentTem_temp=m_nCurrentTem+1;

                this->_SetCurrentTem(CurrentTem_temp);

                WaitSec(6);
            }
            else if(m_nWindSpeed==2){

                int CurrentTem_temp=m_nCurrentTem+1;

                this->_SetCurrentTem(CurrentTem_temp);

               WaitSec(4);
            }
        }
        else if(m_nSettedTem < m_nCurrentTem){//设置的温度小于当前温度
            qDebug()<<"设置温度小于室内温度，室内温度正在降温......";
            if(m_nWindSpeed==0){

                int CurrentTem_temp=m_nCurrentTem-1;
                this->_SetCurrentTem(CurrentTem_temp);
                WaitSec(8);
            }
            else if(m_nWindSpeed==1){

                int CurrentTem_temp=m_nCurrentTem-1;
                this->_SetCurrentTem(CurrentTem_temp);

                WaitSec(6);
            }
            else if(m_nWindSpeed==2){

                int CurrentTem_temp=m_nCurrentTem-1;
                this->_SetCurrentTem(CurrentTem_temp);

                WaitSec(4);
            }
        }
        else{//当温度达到要求的温度时，从控机发送请求停止送风
            qDebug()<<"温度已经达到目标温度，从控机发送停止送风请求";
            QString s="";
            s=s+"request"+"#3#"+roomID+"#3#"+"*#*#;";

            this->_SetUnWindSpeed(3);//把未确认的风速设置为3,表示停风

            sendMessage(s);

            flag3=true;//自动停止的标志
            WaitSec(3);
        }
    }
}

void MainWindow::WaitSec(int s){
    QTime t;
    t.start();
    while(t.elapsed()<1000*s)
        QCoreApplication::processEvents();
}

/*
//返回现在温度
int MainWindow::_GetCurrentTem(){
    qDebug()<<"得到现在温度"<<m_nCurrentTem;
    return m_nCurrentTem;
}
//返回设置的温度
int MainWindow::_GetSettedTem(){
    qDebug()<<"得到设置确认温度为"<<m_nSettedTem;
    return m_nSettedTem;
}
//返回风速
int MainWindow::_GetWindSpeed(){
    qDebug()<<"得到确认风速为"<<m_nWindSpeed;
    return m_nWindSpeed;
}
//返回未确认风速
int MainWindow::_GetUnWindSpeed(){
    qDebug()<<"得到未确认风速为"<<m_nUnWindSpeed;
    return m_nUnWindSpeed;
}
//返回电力用量
double MainWindow::_GetElecUsing(){
    qDebug()<<"得到使用电量为"<<m_fElecUsing;
    return m_fElecUsing;
}
//返回使用金额
double MainWindow::_GetMoneyUsing(){
    qDebug()<<"得到使用金额为"<<m_fMoneyUsing;
    return m_fMoneyUsing;
}

//返回工作模式
int MainWindow::_GetWorkStyle(){
    qDebug()<<"得到工作模式为"<<m_nWorkStyle;
    return m_nWorkStyle;
}
//返回未确认的设置温度
int MainWindow::_GetUnSettedTem(){
    qDebug()<<"得到未确认温度为"<<m_nUnSettedTem;
    return m_nUnSettedTem;
}
*/

void MainWindow::_SetCurrentTem(int nCurrentTem_temp){//设置现在的温度
    m_nCurrentTem=nCurrentTem_temp;
    qDebug()<<"设置现在温度为"<<m_nCurrentTem;
    QString m_nCurrentTemString=QString("%1").arg(m_nCurrentTem);
    ui->work_state_2->setText(m_nCurrentTemString);

    this->counter++;
    if (this->counter == 3) {
        this->tempData.push_back(m_nCurrentTem);
        if (this->tempData.size() > 11) {
            this->tempData.erase(this->tempData.begin());
          }
        this->counter = 0;
        if (this->dataNum != 11)
          this->dataNum++;
        QVector<double> x;
        x.clear();

        for (int i=1; i<=this->dataNum; i++) {
            x.push_back(10-this->dataNum + i);
          }

        ui->table->addGraph();
        ui->table->graph(0)->setData(x, this->tempData);
        ui->table->replot();

        ui->work_state_2->setText(QString::number(m_nCurrentTem));
      }
}

void MainWindow::_SetWindSpeed(int nWindSpeed_temp){//设置风速
    m_nWindSpeed=nWindSpeed_temp;
    QString s;
    qDebug()<<"设置现在的风速为";
    if(m_nWindSpeed==0){
        qDebug()<<"低风";
        ui->set_temp_2->setText("低");

        ui->low_button->setChecked(true);
        //CTemPropel->open();//温度推进线程转变为有送风的模式
    }
    else if(m_nWindSpeed==1){
        qDebug()<<"中风";
        ui->set_temp_2->setText("中");
        //CTemPropel->open();//温度推进线程转变为有送风的模式
        ui->medium_button->setChecked(true);
    }
    else if(m_nWindSpeed==2){
        qDebug()<<"高风";
        ui->set_temp_2->setText("高");
        //CTemPropel->open();//温度推进线程转变为有送风的模式
        ui->high_button->setChecked(true);
    }
    else if(m_nWindSpeed==3){//停止送风，清空风速面板
        qDebug()<<"停";
        ui->work_state->setText("停");
        //CTemPropel->stop();//温度推进线程转变为无送风的模式
        ui->low_button->setChecked(false);
        ui->medium_button->setChecked(false);
        ui->high_button->setChecked(false);
    }
}

void MainWindow::_SetUnWindSpeed(int nUnWindSpeed_temp){
    qDebug()<<"未确认风速设置";
    m_nUnWindSpeed=nUnWindSpeed_temp;
}


void MainWindow::_SetElecUsing(double fElecUsing){
    m_fElecUsing=fElecUsing;
    QString s=QString("%1").arg(fElecUsing);
    qDebug()<<"设置花费电量为"<<s;
    ui->consumption->setText(s);
}


void MainWindow::_SetMoneyUsing(double fMoneyUsing_temp){
    m_fMoneyUsing=fMoneyUsing_temp;
    QString s=QString("%1").arg(m_fMoneyUsing);
    qDebug()<<"设置花费金额为"<<s;
    ui->bill->setText(s);
}

void MainWindow::_SetWorkStyle(int nWorkStyle_temp){
    m_nWorkStyle=nWorkStyle_temp;
    qDebug()<<"设置工作模式为"<<nWorkStyle_temp;
    if(m_nWorkStyle==0){
        //ui->WorkStyle_label->setText("制冷");
        // 设置鼠标样式
        ui->work_state->setText("工作模式：制冷");
        QCursor cursor ;
        QPixmap pixmap(":/ui/Hand1.tga") ;
        cursor = QCursor(pixmap, 0, 0);
        setCursor(cursor);

    }
    else if(m_nWorkStyle==1){
        //ui->WorkStyle_label->setText("制暖");
        // 设置鼠标样式
        ui->work_state->setText("工作模式：制暖");
        QCursor cursor ;
        QPixmap pixmap(":/ui/HoverEnemy.tga") ;
        cursor = QCursor(pixmap, 0, 0);
        setCursor(cursor);
    }
}

void MainWindow::_SetSettedTem(int nSettedTem_temp){//设置现在自己的温度设置
    m_nSettedTem=nSettedTem_temp;
   // ui->SettedTem_lcdNumber->setDigitCount(m_nSettedTem);
}

void MainWindow::_SetUnSettedTem(int nUnSettedTem_temp){
    m_nUnSettedTem=nUnSettedTem_temp;
    QString m_nUnSettedTemString=QString("%1").arg(m_nUnSettedTem);
    ui->set_temp->setText(m_nUnSettedTemString);
}


/* 展示用量 */
void MainWindow::on_report_clicked()
{
  if (this->switchState == OFFWORK) {
      QMessageBox *msgBox = new QMessageBox();
      QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
      for(int i = 0; i < textFieldLabels.size(); i++)
        {
          textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
          //设置文字显示的对齐方式
        }
      msgBox->setWindowTitle("通知");
      QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
      msgBox->setWindowIcon(exeIcon);
      msgBox->setGeometry(550, 350, 400, 358);
      msgBox->setText("请先打开空调再进行设置！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
  else {
      if (ui->report_group->isVisible()) {
        ui->report_group->setVisible(false);
        }
      else {
        ui->speed_group->setVisible(false);
        ui->login_group->setVisible(false);
        ui->model_group->setVisible(false);
        ui->report_group->setVisible(true);
        QPropertyAnimation *pAnimation = new QPropertyAnimation(ui->report_group, "geometry");
        QRect geometry = ui->report_group->geometry();
        pAnimation->setDuration(200);
        pAnimation->setStartValue(QRect(30, 420, 0, 0));
        pAnimation->setEndValue(QRect(geometry.x(), geometry.y(), geometry.width(), geometry.height()));
        pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}

/* 完成登陆 */
void MainWindow::on_loginBtn_clicked()
{
    qDebug()<<"连接认证请求开始......";
    tcpSocket->connectToHost(hostAddress,port);
    roomID=ui->slave_number_field->text();//从控机编号
    ID=ui->password_field->text();//身份证号
    QString s="";//连接信息
    s=s+"request"+"#1#"+roomID+'#'+ID+"#*#*#;";
    qDebug()<<"连接认证请求信息已经发送......";
    sendMessage(s);//直接发送信息
    /*
  int slaveNo = ui->slave_number_field_2->text().toInt();
  QString password = ui->password_field_2->text();
  if(ui->slave_number_field_2->text() == "")
    {
      QMessageBox *msgBox = new QMessageBox();
      QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
      for(int i = 0; i < textFieldLabels.size(); i++)
        {
          textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
          //设置文字显示的对齐方式
        }
      msgBox->setWindowTitle("警告");
      QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
      msgBox->setWindowIcon(exeIcon);
      msgBox->setGeometry(550, 350, 400, 358);
      msgBox->setText("请填写本从机号！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
  else if(ui->password_field_2->text() == "")
    {
      QMessageBox *msgBox = new QMessageBox();
      QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
      for(int i = 0; i < textFieldLabels.size(); i++)
        {
          textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
          //设置文字显示的对齐方式
        }
      msgBox->setWindowTitle("警告");
      QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
      msgBox->setWindowIcon(exeIcon);
      msgBox->setGeometry(550, 350, 400, 358);
      msgBox->setText("密码不能为空！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
    */
}

/* 设置风速 */
void MainWindow::on_windspeed_button_clicked()
{
  if (this->switchState == OFFWORK) {
      QMessageBox *msgBox = new QMessageBox();
      QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
      for(int i = 0; i < textFieldLabels.size(); i++)
        {
          textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
          //设置文字显示的对齐方式
        }
      msgBox->setWindowTitle("通知");
      QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
      msgBox->setWindowIcon(exeIcon);
      msgBox->setGeometry(550, 350, 400, 358);
      msgBox->setText("请先打开空调再进行设置！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
  else {
      if (ui->speed_group->isVisible()) {
          ui->speed_group->setVisible(false);
        }
      else {
          ui->speed_group->setVisible(true);
          ui->login_group->setVisible(false);
          ui->model_group->setVisible(false);
          ui->report_group->setVisible(false);

          QPropertyAnimation *pAnimation = new QPropertyAnimation(ui->speed_group, "geometry");
          QRect geometry = ui->speed_group->geometry();
          pAnimation->setDuration(200);
          pAnimation->setStartValue(QRect(325, 480, 0, 0));
          pAnimation->setEndValue(QRect(geometry.x(), geometry.y(), geometry.width(), geometry.height()));
          pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }

}

/* 设置模式 */
void MainWindow::on_model_button_clicked()
{
  if (this->switchState == OFFWORK) {
      QMessageBox *msgBox = new QMessageBox();
      QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
      for(int i = 0; i < textFieldLabels.size(); i++)
        {
          textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
          //设置文字显示的对齐方式
        }
      msgBox->setWindowTitle("通知");
      QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
      msgBox->setWindowIcon(exeIcon);
      msgBox->setGeometry(550, 350, 400, 358);
      msgBox->setText("请先打开空调再进行设置！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
  else {
      if (ui->model_group->isVisible()) {
          ui->model_group->setVisible(false);
        }
      else {
          ui->model_group->setVisible(true);
          ui->speed_group->setVisible(false);
          ui->login_group->setVisible(false);
          ui->report_group->setVisible(false);

          QPropertyAnimation *pAnimation = new QPropertyAnimation(ui->model_group, "geometry");
          QRect geometry = ui->model_group->geometry();
          pAnimation->setDuration(200);
          pAnimation->setStartValue(QRect(325, 410, 0, 0));
          pAnimation->setEndValue(QRect(geometry.x(), geometry.y(), geometry.width(), geometry.height()));
          pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}

/* 设置布局 */
void MainWindow::arrange() {
  // 美化界面
  setWindowTitle("client");
  setWindowFlags(Qt::FramelessWindowHint | windowFlags());

  // 设置鼠标样式
  QCursor cursor ;
  QPixmap pixmap(":/ui/Hand2.tga") ;
  cursor = QCursor(pixmap, 0, 0);
  setCursor(cursor);

  ui->ccc->setText("");
  ui->ccc->setStyleSheet("QPushButton{border-image: url(:/ui/CCC.png);}");
  ui->ccc->setDisabled(true);
  ui->ccc_2->setText("");
  ui->ccc_2->setStyleSheet("QPushButton{border-image: url(:/ui/CCC.png);}");
  ui->ccc_2->setDisabled(true);
  ui->pushButton->setText("");
  ui->pushButton->setDisabled(true);
  ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/window/back.png);}");
  ui->pushButton_2->setText("");
  ui->pushButton_2->setDisabled(true);
  //ui->pushButton_2->setStyleSheet("QPushButton{border-image: url(:/window/black.png);}");
  ui->close->setText("");
  ui->close->setStyleSheet("QPushButton{border-image: url(:/window/close.png);}"    "QPushButton:hover{border-image: url(:/window/close2.png);}"    "QPushButton:pressed{border-image: url(:/window/close2.png);}");
  ui->min->setText("");
  ui->min->setStyleSheet("QPushButton{border-image: url(:/window/min.png);}"    "QPushButton:hover{border-image: url(:/window/min2.png);}"    "QPushButton:pressed{border-image: url(:/window/min2.png);}");
  ui->logout->setText("");
  ui->logout->setToolTip("登录您的账号");
  ui->logout->setStyleSheet("QPushButton{border-image: url(:/window/14.png);}");
  ui->report->setText("");
  ui->report->setToolTip("查看余额信息");
  ui->report->setStyleSheet("QPushButton{border-image: url(:/window/10.png);}");
  ui->master_button->setText("");
  ui->master_button->setToolTip("开关");
  ui->master_button->setStyleSheet("QPushButton{border-image: url(:/window/switch1.png);}");
  ui->windspeed_button->setText("");
  ui->windspeed_button->setToolTip("可以设置三档风速");
  ui->windspeed_button->setStyleSheet("QPushButton{border-image: url(:/ui/wind2.png);}");
  ui->model_button->setText("");
  ui->model_button->setToolTip("选择制冷或者制热");
  ui->model_button->setStyleSheet("QPushButton{border-image: url(:/ui/mode2.png);}");
  ui->up_temp_button->setText("");
  ui->up_temp_button->setToolTip("升温");
  ui->up_temp_button->setStyleSheet("QPushButton{border-image: url(:/ui/up.png);}");
  ui->down_temp_button->setText("");
  ui->down_temp_button->setToolTip("降温");
  ui->down_temp_button->setStyleSheet("QPushButton{border-image: url(:/ui/down.png);}");
  ui->loginBtn->setText("");
  ui->loginBtn->setStyleSheet("QPushButton{border-image: url(:/window/next2.png);}"    "QPushButton:hover{border-image: url(:/window/next.png);}"    "QPushButton:pressed{border-image: url(:/window/next.png);}");
  ui->warm_button->setStyleSheet("QPushButton{border-image: url(:/ui/warm.png);}");
  ui->cool_button->setStyleSheet("QPushButton{border-image: url(:/ui/cold.png);}");

  ui->table->xAxis->setRange(10, 0);
  ui->table->yAxis->setRange(16, 35);
  ui->table->xAxis->setTickLabelColor(QColor(255, 255, 255));
  ui->table->replot();
  ui->again->setText("");
  ui->again->setToolTip("重置折线图");
  ui->again->setStyleSheet("QPushButton{border-image: url(:/window/8.png);}");
  ui->speed_button->setText("");
  ui->speed_button->setStyleSheet("QPushButton{border-image: url(:/window/ok.png);}");

  ui->speed_group->setVisible(false);
  ui->login_group->setVisible(false);
  ui->model_group->setVisible(false);
  ui->report_group->setVisible(false);

  ui->set_temp->setText("--");
  ui->set_temp_2->setText("--");
  ui->login_state->setText("");
  ui->login_state->setDisabled(true);
  ui->login_state->setToolTip("未登录");
  ui->login_state->setStyleSheet("QPushButton{border-image: url(:/window/20.png);}");
  ui->work_state_2->setText(QString::number(this->m_nCurrentTem));
  ui->work_state->setText("");

//  ui->pushButton->setText("");
//  ui->pushButton->setDisabled(true);
//  ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/window/back.png);}");
//  ui->pushButton_2->setText("");
//  ui->pushButton_2->setDisabled(true);
//  //ui->pushButton_2->setStyleSheet("QPushButton{border-image: url(:/window/black.png);}");
//  ui->close->setText("");
//  ui->close->setStyleSheet("QPushButton{border-image: url(:/window/close.png);}"    "QPushButton:hover{border-image: url(:/window/close2.png);}"    "QPushButton:pressed{border-image: url(:/window/close2.png);}");
//  ui->min->setText("");
//  ui->min->setStyleSheet("QPushButton{border-image: url(:/window/min.png);}"    "QPushButton:hover{border-image: url(:/window/min2.png);}"    "QPushButton:pressed{border-image: url(:/window/min2.png);}");

//  ui->table->xAxis->setRange(10, 0);
//  ui->table->yAxis->setRange(16, 35);
//  ui->table->xAxis->setTickLabelColor(QColor(255, 255, 255));
//  ui->table->replot();
//  ui->again->setText("");
//  ui->again->setToolTip("重置折线图");
//  ui->again->setStyleSheet("QPushButton{border-image: url(:/window/8.png);}");
//  ui->speed_button->setText("");
//  ui->speed_button->setStyleSheet("QPushButton{border-image: url(:/window/ok.png);}");
//  ui->login_state->setText("");
//  ui->login_state->setDisabled(true);
//  ui->login_state->setToolTip("未登录");
//  ui->login_state->setStyleSheet("QPushButton{border-image: url(:/window/20.png);}");
//  ui->master_button->setText("");
//  ui->master_button->setToolTip("开关");
//  ui->master_button->setStyleSheet("QPushButton{border-image: url(:/window/switch1.png);}");
//  ui->set_temp->setText("--");
//  ui->set_temp_2->setText("--");
//  ui->work_state_2->setText(QString::number(this->m_nCurrentTem));
//  ui->work_state->setText("");
}

/* 注销并且关闭窗口 */
void MainWindow::on_close_clicked()
{
  if (loginState == ONLINE) {
      QString s="";
      s=s+"request"+"#4#"+roomID+"#*#*#*#;";
      qDebug()<<"关机请求信息已经发送";
      sendMessage(s);
      g_time->stop();//定时器停止

      tcpSocket->close();//TCP socket 关闭
      m_nWindSpeed=3;//把风速设置为停风
      //CTemPropel->stop();//温度推进线程将改为无连接模式
      //this->close();
      close();
    }
  else {
      close();
    }
}

/* 开电源 */
void MainWindow::on_master_button_clicked()
{
    if (this->switchState == OFFWORK) {
        this->switchState = ONWORK;
        ui->master_button->setStyleSheet("QPushButton{border-image: url(:/window/switch2.png);}");

      }
    else {
        this->switchState = OFFWORK;
        ui->master_button->setStyleSheet("QPushButton{border-image: url(:/window/switch1.png);}");
      }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_up_temp_button_clicked()
{
  if (this->switchState == OFFWORK) {
      QMessageBox *msgBox = new QMessageBox();
      QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
      for(int i = 0; i < textFieldLabels.size(); i++)
        {
          textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
          //设置文字显示的对齐方式
        }
      msgBox->setWindowTitle("通知");
      QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
      msgBox->setWindowIcon(exeIcon);
      msgBox->setGeometry(550, 350, 400, 358);
      msgBox->setText("请先打开空调再进行设置！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
  else {
      if(m_nWorkStyle==1){//制热
          if(m_nUnSettedTem<initTem){
              this->_SetUnSettedTem(++m_nUnSettedTem);
              QString s="";
              QString s_temp1=QString::number(m_nUnSettedTem,10);//把int型的未确认温度转变为字符串型
              if(m_nUnSettedTem<=25)
                  s=s+"request"+"#2#"+roomID+"#0#"+s_temp1+"#*#;";
              else s=s+"request"+"#2#"+roomID+"#1#"+s_temp1+"#*#;";
              qDebug()<<"温度请求信息已经发送";
              sendMessage(s);
          }
      }
      else if(m_nWorkStyle==0){//制冷
          if(m_nUnSettedTem<25){
              this->_SetUnSettedTem(++m_nUnSettedTem);
              QString s="";
              QString s_temp1=QString::number(m_nUnSettedTem,10);//把int型的未确认温度转变为字符串型
              if(m_nUnSettedTem<=25)
                  s=s+"request"+"#2#"+roomID+"#0#"+s_temp1+"#*#;";
              else s=s+"request"+"#2#"+roomID+"#1#"+s_temp1+"#*#;";
              qDebug()<<"温度请求信息已经发送";
              sendMessage(s);
          }
      }
    }

}

void MainWindow::on_down_temp_button_clicked()
{
  if (this->switchState == OFFWORK) {
      QMessageBox *msgBox = new QMessageBox();
      QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
      for(int i = 0; i < textFieldLabels.size(); i++)
        {
          textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
          //设置文字显示的对齐方式
        }
      msgBox->setWindowTitle("通知");
      QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
      msgBox->setWindowIcon(exeIcon);
      msgBox->setGeometry(550, 350, 400, 358);
      msgBox->setText("请先打开空调再进行设置！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
  else {
      if(m_nWorkStyle==0){//制冷
          if(m_nUnSettedTem>initTem){
              this->_SetUnSettedTem(--m_nUnSettedTem);

              QString s="";
              QString s_temp1=QString::number(m_nUnSettedTem,10);//把int型的未确认温度转变为字符串型
              if(m_nUnSettedTem<=25)
                  s=s+"request"+"#2#"+roomID+"#0#"+s_temp1+"#*#;";
              else s=s+"request"+"#2#"+roomID+"#1#"+s_temp1+"#*#;";
              qDebug()<<"温度请求信息已经发送";
              sendMessage(s);
          }
      }
      else if(m_nWorkStyle==1){//制热
          if(m_nUnSettedTem>26){
              this->_SetUnSettedTem(++m_nUnSettedTem);

              QString s="";
              QString s_temp1=QString::number(m_nUnSettedTem,10);//把int型的未确认温度转变为字符串型
              if(m_nUnSettedTem<=25)
                  s=s+"request"+"#2#"+roomID+"#0#"+s_temp1+"#*#;";
              else s=s+"request"+"#2#"+roomID+"#1#"+s_temp1+"#*#;";
              qDebug()<<"温度请求信息已经发送";
              sendMessage(s);
          }
      }
    }
}

void MainWindow::on_speed_button_clicked()
{
  if (ui->high_button->isChecked()){
      this->_SetUnWindSpeed(2);
      QString s="";
      s=s+"request"+"#3#"+roomID+"#2#"+"*#*#;";
      qDebug()<<"高风速请求信息已经发送";
      sendMessage(s);
  }
  else if(ui->medium_button->isChecked()){
      this->_SetUnWindSpeed(1);
      QString s="";
      s=s+"request"+"#3#"+roomID+"#1#"+"*#*#;";
      qDebug()<<"中风速请求信息已经发送";
      sendMessage(s);
  }
  else if(ui->low_button->isChecked()){
      this->_SetUnWindSpeed(0);
      QString s="";
      s=s+"request"+"#3#"+roomID+"#0#"+"*#*#;";
      qDebug()<<"低风速请求信息已经发送";
      sendMessage(s);
  }
}

void MainWindow::on_logout_clicked()
{
  if (this->switchState == OFFWORK) {
      QMessageBox *msgBox = new QMessageBox();
      QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
      for(int i = 0; i < textFieldLabels.size(); i++)
        {
          textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
          //设置文字显示的对齐方式
        }
      msgBox->setWindowTitle("通知");
      QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
      msgBox->setWindowIcon(exeIcon);
      msgBox->setGeometry(550, 350, 400, 358);
      msgBox->setText("请先打开空调再进行设置！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
  else {
      if (this->loginState == OFFLINE) {
          if (ui->login_group->isVisible()) {
              ui->login_group->setVisible(false);
            }
          else {
              ui->login_group->setVisible(true);
              ui->speed_group->setVisible(false);
              ui->model_group->setVisible(false);
              ui->report_group->setVisible(false);

              QPropertyAnimation *pAnimation = new QPropertyAnimation(ui->login_group, "geometry");
              QRect geometry = ui->login_group->geometry();
              pAnimation->setDuration(200);
              pAnimation->setStartValue(QRect(30, 480, 0, 0));
              pAnimation->setEndValue(QRect(geometry.x(), geometry.y(), geometry.width(), geometry.height()));
              pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            }
        }
      else {
          QString s="";
          s=s+"request"+"#4#"+roomID+"#*#*#*#;";
          qDebug()<<"关机请求信息已经发送";
          sendMessage(s);
          g_time->stop();//定时器停止

          tcpSocket->close();//TCP socket 关闭
          m_nWindSpeed=3;//把风速设置为停风
          //CTemPropel->stop();//温度推进线程将改为无连接模式
          //this->close();
        }
    }
}

void MainWindow::on_warm_button_clicked()
{
  QMessageBox *msgBox = new QMessageBox();
  QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
  for(int i = 0; i < textFieldLabels.size(); i++)
    {
      textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
      //设置文字显示的对齐方式
    }
  msgBox->setWindowTitle("通知");
  QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
  msgBox->setWindowIcon(exeIcon);
  msgBox->setGeometry(550, 350, 400, 358);
  msgBox->setText("选择模式：制热");
  msgBox->addButton("确定", QMessageBox::AcceptRole);
  msgBox->exec();

    if (this->m_nWorkStyle == 0) {
        // 设置鼠标样式
        QCursor cursor ;
        QPixmap pixmap(":/ui/Hand1.tga") ;
        cursor = QCursor(pixmap, 0, 0);
        setCursor(cursor);

        QMessageBox *msgBox = new QMessageBox();
        QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
        for(int i = 0; i < textFieldLabels.size(); i++)
          {
            textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
            //设置文字显示的对齐方式
          }
        msgBox->setWindowTitle("通知");
        QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
        msgBox->setWindowIcon(exeIcon);
        msgBox->setGeometry(550, 350, 400, 358);
        msgBox->setText("与中央空调的模式不匹配！");
        msgBox->addButton("确定", QMessageBox::AcceptRole);
        msgBox->exec();
        return;
      }
    else {
        // 设置鼠标样式
        QCursor cursor ;
        QPixmap pixmap(":/ui/HoverEnemy.tga") ;
        cursor = QCursor(pixmap, 0, 0);
        setCursor(cursor);
      }
}

void MainWindow::on_cool_button_clicked()
{
  QMessageBox *msgBox = new QMessageBox();
  QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
  for(int i = 0; i < textFieldLabels.size(); i++)
    {
      textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
      //设置文字显示的对齐方式
    }
  msgBox->setWindowTitle("通知");
  QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
  msgBox->setWindowIcon(exeIcon);
  msgBox->setGeometry(550, 350, 400, 358);
  msgBox->setText("选择模式：制冷");
  msgBox->addButton("确定", QMessageBox::AcceptRole);
  msgBox->exec();

  if (this->m_nWorkStyle == 1) {
      // 设置鼠标样式
      QCursor cursor ;
      QPixmap pixmap(":/ui/HoverEnemy.tga") ;
      cursor = QCursor(pixmap, 0, 0);
      setCursor(cursor);

      QMessageBox *msgBox = new QMessageBox();
      QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
      for(int i = 0; i < textFieldLabels.size(); i++)
        {
          textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
          //设置文字显示的对齐方式
        }
      msgBox->setWindowTitle("通知");
      QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
      msgBox->setWindowIcon(exeIcon);
      msgBox->setGeometry(550, 350, 400, 358);
      msgBox->setText("与中央空调的模式不匹配！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
  else {
      // 设置鼠标样式
      QCursor cursor ;
      QPixmap pixmap(":/ui/Hand1.tga") ;
      cursor = QCursor(pixmap, 0, 0);
      setCursor(cursor);
    }
}
