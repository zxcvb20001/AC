#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  setSlaveNo(INDEX); //初始化从机编号
  setFixedSize(485, 550);

  //参数初始化
  setLoginState(logout_const); //初始化为未登录
  setWorkState(my_idle); //初始设为未工作
  setWorkMode(cool); //默认制冷
  setTemp(22); //初始化工作温度
  setCurrentTemp(defaultTemp_summer);
  setTurnOn(turn_off); //初始化开关机状态

  arrange();

  this->counter = 0;
  this->dataNum = 0;
  this->tempData.clear();
  this->windSpeed = lowspeed;
  this->sendDataFrequency = 1;
  sendCnt = 0;
  slotCnt = 1;
  this->consumption = 0;
  this->bill = 0;
  this->isRemindMode = 0;

  //从控机界面初始化
  ui->set_temp->setText("--");
  ui->set_temp_2->setText("--");
  ui->login_state->setText("");
  ui->login_state->setDisabled(true);
  ui->login_state->setToolTip("未登录");
  ui->login_state->setStyleSheet("QPushButton{border-image: url(:/window/20.png);}");
  ui->work_state_2->setText(QString::number(this->currentTemp));
  ui->work_state_2->setText("");
  ui->current_temp_integer->setText(QString::number(this->currentTemp, 10, 0));
  QStringList tmpList = QString::number(this->currentTemp, 10, 1).split(".");
  ui->current_temp_fraction->setText("."+tmpList[1]);

  ui->model_group->setVisible(false);
  ui->current_choice->setText("当前模式：制冷");

  ui->speed_group->setVisible(false);
  ui->low_button->setChecked(true);

  ui->login_group->setVisible(false);
  ui->password_field->setEchoMode(QLineEdit::Password);

  ui->consumption->setText("--");
  ui->bill->setText("--");
  ui->report_group->setVisible(false);
  //  ui->speed_button->setObjectName("speed");
  //  ui->speed_button->setStyleSheet("#speed { color: rgb(0, 0, 127);background-image: url();} ");

  //槽函数
  tcpsocket = new QTcpSocket(this);
  connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
  connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
  // 自定义最小化按钮
  connect(ui->min, SIGNAL(clicked()), this, SLOT(showMinimized()));

  QTimer *timer = new QTimer(this);
  timer->start(1000);
  connect(timer,SIGNAL(timeout()),this,SLOT(timeslot()));

  QTimer *timer1 = new QTimer(this);
  timer1->start(1);
  connect(timer1,SIGNAL(timeout()),this,SLOT(DealWithBag()));
}

void MainWindow::arrange()
{
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
}

MainWindow::~MainWindow()
{
  send_message.clear();
  send_message = "request#5#"+QString::number(slave_no)+"#*#*#*#;";
  sendMessage();
  tcpsocket->close();
  delete ui;
}

void MainWindow::newConnect()//连接函数
{
  tcpsocket = new QTcpSocket(this);
  connect(tcpsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
  connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
  blockSize = 0; //初始化其为0
  tcpsocket->open(QIODevice::ReadWrite);
  tcpsocket->connectToHost(IPADDRESS, 6666);//设置客户端的端口号
  connect(tcpsocket,SIGNAL(disconnected()),tcpsocket,SLOT(deleteLater()));
}

void MainWindow::sendMessage()//发送信息函数
{
  //QString string = ui->lineEdit->text();
  //     QByteArray message;
  //     QDataStream out(&message,QIODevice::WriteOnly);
  //     out.setVersion(QDataStream::Qt_5_8);
  //     out<<send_message;
  tcpsocket->write(send_message.toStdString().c_str(),strlen(send_message.toStdString().c_str()));
}

void MainWindow::readMessage()//读取信息函数
{
  //    QDataStream in(tcpsocket);
  //    in.setVersion(QDataStream::Qt_5_8);
  //    //设置数据流版本，这里要和服务器端相同
  //    blockSize = 0;
  //    if(blockSize == 0) //如果是刚开始接收数据
  //    {
  //        //判断接收的数据是否有两字节，也就是文件的大小信息
  //        //如果有则保存到blockSize变量中，没有则返回，继续接收数据
  //        if(tcpsocket->bytesAvailable() < (int)sizeof(quint16)) return;
  //           in >> blockSize;
  //     }
  //     if(tcpsocket->bytesAvailable() < blockSize) return;
  //     //如果没有得到全部的数据，则返回，继续接收数据
  //     in >> rec_message;
  QByteArray tt = this->tcpsocket->readAll();
  this->rec_message = QVariant(tt).toString();
  qDebug() << "收到数据" << rec_message << endl;

  //分析收到的信息
  if(rec_message == "") return;
  QStringList list = rec_message.split(";");//切割数据
  int len = list.size();
  for(int i = 0; i < len; i++)
    {
      qDebug() << list[i];
      DataBagList.enqueue(list[i]);
    }
  this->rec_message.clear();
}

void MainWindow::DealWithBag()
{
  while(!DataBagList.empty())
    {
      qDebug() << "bagbag!!";
      QString message = this->DataBagList.head();
      if(message == "")
        {
          this->DataBagList.dequeue();
          continue;
        }
      QStringList message_List = message.split("#");

      //如果符合自己的从机号或者是一条广播指令
      if(message_List[0] == "instruction" && (message_List[2] == QString::number(this->slave_no) || message_List[2] == "*"))
        DealWithInstruction(message);
      else if(message_List[2] == QString::number(this->slave_no))
        DealWithInformation(message);
      else
        qDebug() << "Bad message from central controller...";
      this->DataBagList.dequeue();
    }
}


void MainWindow::DealWithInstruction(QString rec_message)
{
  qDebug() << "instrcution" << rec_message;
  QStringList message_List = rec_message.split("#");
  if(message_List[1] == "1")//连接认证
    {
      if(message_List[3] == "0")
        {
          this->loginState = logout_const;
          QMessageBox *msgBox = new QMessageBox();
          QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
          for(int i = 0; i < textFieldLabels.size(); i++)
            {
              textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
              //设置文字显示的对齐方式
            }
          //msgBox->setWindowTitle("警告");
          QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
          msgBox->setWindowIcon(exeIcon);
          msgBox->setGeometry(870,350,400,358);
          //msgBox->setText("连接认证失败或连接断开！");
          msgBox->addButton("确定", QMessageBox::AcceptRole);
          //msgBox->exec();
        }
      else
        {
          QMessageBox *msgBox = new QMessageBox();
          QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
          for(int i = 0; i < textFieldLabels.size(); i++)
            {
              textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
              //设置文字显示的对齐方式
            }
          //msgBox->setWindowTitle("通知");
          QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
          msgBox->setWindowIcon(exeIcon);
          msgBox->setGeometry(550, 350, 400, 358);
          //msgBox->setText("认证成功！");
          this->loginState = login_const;
          this->settemp = message_List[5].toInt();
          //msgBox->addButton("确定", QMessageBox::AcceptRole);
          //msgBox->exec();
          ui->login_group->setVisible(false);

          ui->password_field->clear();
          ui->logout->setToolTip(" 注销您的账号");
          ui->logout->setStyleSheet("QPushButton{border-image: url(:/window/15.png);}");
          ui->login_state->setToolTip("连接成功");
          ui->login_state->setStyleSheet("QPushButton{border-image: url(:/window/20.png);}");
        }
    }
  else if(message_List[1] == "2")//温控指令
    {
      if(message_List[3] == "0")
        {
          qDebug() << "sdaasdadsa:" << this->isRemindMode;
          this->workState = my_idle;
          if(this->isRemindMode == 0)
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
              msgBox->setText("请修改工作模式\n与中央系统模式不同！");
              /* 不同不同不同不同不同 */
              if (this->workMode == cool) {
                  this->currentTemp = defaultTemp_winter;
                  this->settemp = defaultWarmTemp;
                  this->workMode = warm;

                  // 设置鼠标样式
                  QCursor cursor ;
                  QPixmap pixmap(":/ui/HoverEnemy.tga") ;
                  cursor = QCursor(pixmap, 0, 0);
                  setCursor(cursor);
                }
              else if (this->workMode == warm){
                  this->currentTemp = 35;
                  this->workMode = cool;
                  this->settemp = defaultCoolTemp;

                  // 设置鼠标样式
                  QCursor cursor ;
                  QPixmap pixmap(":/ui/Hand1.tga") ;
                  cursor = QCursor(pixmap, 0, 0);
                  setCursor(cursor);
                }
              //                if(this->workMode == cool)
              //                    this->workMode = warm;
              //                else
              //                    this->workMode = cool;
              msgBox->addButton("确定", QMessageBox::AcceptRole);
              msgBox->exec();
              this->isRemindMode = 1;
            }
          //qDebug() << "工作模式无效,请修改工作模式！";
        }
      else
        {
          //正常工作
//          if (this->workMode == warm) {
//              if (this->currentTemp <= this->settemp)
//                this->workState = my_run;
//            }
//          else {
//              if (this->currentTemp >= this->settemp)
//                this->workState = my_run;
//            }
           this->workState = my_run;
        }
    }
  else if(message_List[1] == "3")//风速指令
    {
      if(message_List[3] == "0")//没有调度上
        {
          this->workState = my_idle;
        }
    }
  else if(message_List[1] == "4")//刷新更新频率
    {
      int frequency = message_List[4].toInt();
      this->sendDataFrequency = frequency;
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
      msgBox->setText("刷新频率已经更改为"+message_List[4]);
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();

      qDebug("刷新频率已经更改\n");
    }
}

void MainWindow::DealWithInformation(QString rec_message)
{
  qDebug() << "information" << rec_message;
  QStringList list = rec_message.split("#");
  this->consumption = list[3].toDouble();
  this->bill = list[4].toDouble();
  ui->consumption->setText(list[3]);
  ui->bill->setText(list[4]);
}

void MainWindow::displayError(QAbstractSocket::SocketError)
{
  qDebug() << tcpsocket->errorString(); //输出错误信息
}

int MainWindow::getWorkMode()
{
  return this->workMode;
}

void MainWindow::setWorkMode(int tt)
{
  this->workMode = tt;
  if (tt == 0) {
      QCursor cursor ;
      QPixmap pixmap(":/ui/Hand1.tga") ;
      cursor = QCursor(pixmap, 0, 0);
      setCursor(cursor);


    }
  else {
      QCursor cursor ;
      QPixmap pixmap(":/ui/HoverEnemy.tga") ;
      cursor = QCursor(pixmap, 0, 0);
      setCursor(cursor);
    }
}

double MainWindow::getCurrentTemp()
{
  return this->currentTemp;
}

void MainWindow::setCurrentTemp(double tt)
{
  this->currentTemp = tt;
}

double MainWindow::getConsumpt()
{
  return this->consumption;
}

double MainWindow::getBill()
{
  return this->bill;
}

int MainWindow::getLoginState()
{
  return this->loginState;
}

int MainWindow::getSettemp()
{
  return this->settemp;
}

int MainWindow::getWindSpeed()
{
  return this->windSpeed;
}

int MainWindow::getWorkState()
{
  return this->workState;
}

void MainWindow::setLoginState(int tt)
{
  this->loginState = tt;
}

void MainWindow::setWorkState(int tt)
{
  this->workState = tt;
  if (tt == 0) {
      QCursor cursor ;
      QPixmap pixmap(":/ui/Hand1.tga") ;
      cursor = QCursor(pixmap, 0, 0);
      setCursor(cursor);
    }
  else {
      QCursor cursor ;
      QPixmap pixmap(":/ui/HoverEnemy.tga") ;
      cursor = QCursor(pixmap, 0, 0);
      setCursor(cursor);
    }
}

void MainWindow::setWindSpeed(int tt)
{
  this->windSpeed = tt;
}

void MainWindow::setTemp(int tt)
{
  this->settemp = tt;
}

int MainWindow::getSlaveNo()
{
  return this->slave_no;
}

void MainWindow::setSlaveNo(int tt)
{
  this->slave_no = tt;
}

int MainWindow::getTurnOn()
{
  return this->turnOn;
}

void MainWindow::setTurnOn(int tt)
{
  this->turnOn = tt;
}

void MainWindow::on_master_button_clicked() //关闭从控机
{
  //qDebug() << "close";
  QMessageBox *msgBox = new QMessageBox();
  QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
  for(int i = 0; i < textFieldLabels.size(); i++)
    {
      textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
      //设置文字显示的对齐方式
    }

  msgBox->setWindowTitle("总电源");
  QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
  msgBox->setGeometry(550,350,400,358);
  msgBox->setWindowIcon(exeIcon);
  int turn_state = this->getTurnOn();
  if(turn_state == turn_on)
    {
      msgBox->setText(QString("确定断开电源？"));
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->addButton("取消", QMessageBox::RejectRole);
      //msgBox->setStyleSheet("background-image:url();");
      if(msgBox->exec() == QMessageBox::AcceptRole)
        {
          ui->master_button->setStyleSheet("QPushButton{border-image: url(:/window/switch1.png);}");
          this->setTurnOn(turn_off);
          this->setLoginState(logout_const);
          send_message.clear();
          send_message = "request#5#"+QString::number(slave_no)+"#*#*#*#;";
          sendMessage();
          tcpsocket->close();
          this->setWindowTitle("从控机"+QString::number(this->slave_no)+"-关机");
        }
    }
  else
    {
      ui->master_button->setStyleSheet("QPushButton{border-image: url(:/window/switch2.png);}");
      this->setTurnOn(turn_on);
      this->setWindowTitle("从控机"+QString::number(this->slave_no)+"-开机");
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
  msgBox->setWindowTitle("模式");
  msgBox->setGeometry(550, 350, 400, 358);
  msgBox->setText("选择模式：制冷");
  msgBox->addButton("确定", QMessageBox::AcceptRole);
  msgBox->addButton("取消", QMessageBox::RejectRole);
  //msgBox->setStyleSheet("background-image:url();");
  if(msgBox->exec() == QMessageBox::AcceptRole)
    {
      this->setWorkMode(cool);
      ui->model_group->setVisible(false);
      this->settemp = 22;
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
  msgBox->setWindowTitle("模式");
  QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
  msgBox->setWindowIcon(exeIcon);
  msgBox->setGeometry(550, 350, 400, 358);
  msgBox->setText("选择模式：制热");
  msgBox->addButton("确定", QMessageBox::AcceptRole);
  msgBox->addButton("取消", QMessageBox::RejectRole);
  //msgBox->setStyleSheet("background-image:url();");
  if(msgBox->exec() == QMessageBox::AcceptRole)
    {
      this->setWorkMode(warm);
      ui->model_group->setVisible(false);
      this->settemp = 28;
    }
}

void MainWindow::on_model_button_clicked()
{
  if(this->turnOn == turn_off)
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
      msgBox->setText("请先打开空调再进行设置！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
  if(ui->model_group->isVisible() == true)
    ui->model_group
        ->setVisible(false);
  else
    {
      ui->model_group->setVisible(true);
      ui->login_group->setVisible(false);
      ui->speed_group->setVisible(false);
      ui->report_group->setVisible(false);
      QPropertyAnimation *pAnimation = new QPropertyAnimation(ui->model_group, "geometry");
      QRect geometry = ui->model_group->geometry();
      pAnimation->setDuration(200);
      pAnimation->setStartValue(QRect(325, 420, 0, 0));
      pAnimation->setEndValue(QRect(geometry.x(), geometry.y(), geometry.width(), geometry.height()));
      pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void MainWindow::on_speed_button_clicked()
{
  QString description;
  int valid = 1, setSpeed = 0;
  if(ui->low_button->isChecked())
    {
      description = "低速风";
      setSpeed = lowspeed;
    }
  else if(ui->medium_button->isChecked())
    {
      description = "中速风";
      setSpeed = mediumspeed;
    }
  else if(ui->high_button->isChecked())
    {
      description = "高速风";
      setSpeed = highspeed;
    }
  else valid = 0;

  if(!valid)//未选中任何风速
    {
      QMessageBox *msgBox = new QMessageBox();
      QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
      for(int i = 0; i < textFieldLabels.size(); i++)
        {
          textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
          //设置文字显示的对齐方式
        }
      msgBox->setWindowTitle("警告");
      msgBox->setGeometry(550, 350, 400, 358);
      QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
      msgBox->setWindowIcon(exeIcon);
      msgBox->setText("请选择风速！");
      msgBox->addButton("确定", QMessageBox::ApplyRole);
      msgBox->exec(); //要加执行函数，否则会不弹出对话框
      return;
    }
  QMessageBox *msgBox = new QMessageBox();
  QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
  for(int i = 0; i < textFieldLabels.size(); i++)
    {
      textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
      //设置文字显示的对齐方式
    }
  msgBox->setWindowTitle("风速调节");
  QIcon exeIcon(":/slave_surface.ico");//添加程序的图标
  msgBox->setWindowIcon(exeIcon);
  msgBox->setGeometry(550, 350, 400, 358);
  msgBox->setText("风速设置"+description+"?");
  msgBox->addButton("确定", QMessageBox::AcceptRole);
  msgBox->addButton("取消", QMessageBox::RejectRole);
  //msgBox->setStyleSheet("background-image:url();");
  if(msgBox->exec() == QMessageBox::AcceptRole)
    {
      this->setWindSpeed(setSpeed);
      ui->speed_group->setVisible(false);
    }
  //    QMessageBox::StandardButton rb = QMessageBox::information(this, "通知", "确定设置"+description+"?", QMessageBox::Yes|QMessageBox::Yes, QMessageBox::No);
  //    //rb.setStyleSheet("background-image:url();");
  //    if(rb == QMessageBox::Yes )
  //    {
  //        this->setWindSpeed(setSpeed);
  //        ui->speed_group->setVisible(false);
  //    }
}

void MainWindow::on_windspeed_button_clicked()
{
  if(this->turnOn == turn_off)
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
      msgBox->setText("请先打开空调再进行设置！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
  if(ui->speed_group->isVisible() == true)
    {
      ui->speed_group->setVisible(false);
    }
  else
    {
      ui->login_group->setVisible(false);
      ui->speed_group->setVisible(true);
      ui->model_group->setVisible(false);
      ui->report_group->setVisible(false);
      QPropertyAnimation *pAnimation = new QPropertyAnimation(ui->speed_group, "geometry");
      QRect geometry = ui->speed_group->geometry();
      pAnimation->setDuration(200);
      pAnimation->setStartValue(QRect(325, 490, 0, 0));
      pAnimation->setEndValue(QRect(geometry.x(), geometry.y(), geometry.width(), geometry.height()));
      pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void MainWindow::on_up_temp_button_clicked() //调高设置温度
{
  if(this->turnOn == turn_off)
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
      msgBox->setText("请先打开空调再进行设置！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
  int workstate = this->getWorkMode();
  int set_temp = this->getSettemp();
  if(workstate == cool || workstate == my_idle)
    {
      if(set_temp == 25)
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
          msgBox->setText("设定温度应在18-25度");
          msgBox->addButton("确定", QMessageBox::AcceptRole);
          msgBox->exec();
        }
      else
        {
          this->setTemp(set_temp+1);
        }
    }
  else
    {
      if(set_temp == 30)
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
          msgBox->setText("设定温度应在25-30度！");
          msgBox->addButton("确定", QMessageBox::AcceptRole);
          msgBox->exec();
        }
      else
        {
          this->setTemp(set_temp+1);
        }
    }
}

void MainWindow::on_down_temp_button_clicked() //降低设置温度
{
  if(this->turnOn == turn_off)
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
      msgBox->setText("请先打开空调再进行设置！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }

  int workstate = this->getWorkMode();
  int set_temp = this->getSettemp();
  qDebug() << set_temp;
  if(workstate == cool || workstate == my_idle)
    {
      if(set_temp == 18)
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
          msgBox->setText("设定温度应在18-25度");
          msgBox->addButton("确定", QMessageBox::AcceptRole);
          msgBox->exec();
        }
      else
        {
          this->setTemp(set_temp-1);
        }
    }
  else
    {
      if(set_temp == 25)
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
          msgBox->setText("设定温度应在25-30度！");
          msgBox->addButton("确定", QMessageBox::AcceptRole);
          msgBox->exec();
        }
      else
        {
          this->setTemp(set_temp-1);
        }
    }
}

void MainWindow::on_logout_clicked()
{
  int login_state = this->getLoginState();
  if(login_state == login_const)
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
      msgBox->setText("确定要注销？");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->addButton("取消", QMessageBox::RejectRole);
      if(msgBox->exec() == QMessageBox::AcceptRole)
        {
          this->setLoginState(logout_const);
          send_message.clear();
          send_message = "request#5#"+QString::number(slave_no)+"#*#*#*#;";
          sendMessage();
          int i = 10000;
          while (i--)
          {}
          tcpsocket->close();

          ui->login_state->setToolTip("未登录");
          ui->login_state->setStyleSheet("QPushButton{border-image: url(:/window/20.png);}");
          ui->logout->setToolTip("登录您的账号");
          ui->logout->setStyleSheet("QPushButton{border-image: url(:/window/14.png);}");
        }
    }
  else if(this->turnOn == turn_off)
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
      msgBox->setText("请先打开空调再进行设置！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
    }
  else
    {
      if(ui->login_group->isVisible() == false)
        {
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
      else
        ui->login_group->setVisible(false);
    }
}

void MainWindow::on_report_clicked()
{
  if(ui->report_group->isVisible() == true)
    {
      ui->report_group->setVisible(false);
    }
  else
    {
      ui->login_group->setVisible(false);
      ui->speed_group->setVisible(false);
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

void MainWindow::on_loginBtn_clicked()
{
  int slaveNo = ui->slave_number_field->text().toInt();
  QString password = ui->password_field->text();
  if(slaveNo != this->slave_no || ui->slave_number_field->text() == "")
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
      msgBox->setText("请填写从机号！");
      msgBox->addButton("确定", QMessageBox::AcceptRole);
      msgBox->exec();
      return;
    }
  else if(ui->password_field->text() == "")
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
  send_message.clear();
  send_message = "request#1#"+QString::number(slave_no)+"#"+password+"#*#*#;";//改了

  newConnect();
  sendMessage();
}

/* 自定义关闭按钮 */
void MainWindow::on_close_clicked()
{
  int login_state = this->getLoginState();
  if(login_state == login_const)
    {
      on_logout_clicked();
    }
  close();
}

void MainWindow::on_again_clicked()
{
  QVector<double> x, y;
  x.clear();
  y.clear();
  this->tempData.clear();
  this->dataNum = 0;
  ui->table->addGraph();
  ui->table->graph(0)->setData(x, y);
  ui->table->replot();
}
