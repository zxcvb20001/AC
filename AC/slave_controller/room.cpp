#include "room.h"
#include "ui_room.h"
#include "sendthread.h"
#include "global.h"

Room::Room(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Room)
{
  ui->setupUi(this);
  setFixedSize(485, 550);

  arrange();
  this->loginState = OFFLINE;
  this->switchState = OFFWORK;

  /* 初始值 */
  m_nUnSettedTem = 22;
  this->m_nCurrentTem = 35;

}

//返回现在温度
int Room::_GetCurrentTem(){
    qDebug()<<"得到现在温度"<<m_nCurrentTem;
    return m_nCurrentTem;
}
//返回设置的温度
int Room::_GetSettedTem(){
    qDebug()<<"得到设置确认温度为"<<m_nSettedTem;
    return m_nSettedTem;
}
//返回风速
int Room::_GetWindSpeed(){
    qDebug()<<"得到确认风速为"<<m_nWindSpeed;
    return m_nWindSpeed;
}
//返回未确认风速
int Room::_GetUnWindSpeed(){
    qDebug()<<"得到未确认风速为"<<m_nUnWindSpeed;
    return m_nUnWindSpeed;
}
//返回电力用量
double Room::_GetElecUsing(){
    qDebug()<<"得到使用电量为"<<m_fElecUsing;
    return m_fElecUsing;
}
//返回使用金额
double Room::_GetMoneyUsing(){
    qDebug()<<"得到使用金额为"<<m_fMoneyUsing;
    return m_fMoneyUsing;
}

//返回工作模式
int Room::_GetWorkStyle(){
    qDebug()<<"得到工作模式为"<<m_nWorkStyle;
    return m_nWorkStyle;
}
//返回未确认的设置温度
int Room::_GetUnSettedTem(){
    qDebug()<<"得到未确认温度为"<<m_nUnSettedTem;
    return m_nUnSettedTem;
}


void Room::_SetCurrentTem(int nCurrentTem_temp){//设置现在的温度
    m_nCurrentTem=nCurrentTem_temp;
    qDebug()<<"设置现在温度为"<<m_nCurrentTem;
    QString m_nCurrentTemString=QString("%1").arg(m_nCurrentTem);
    ui->CurrentTem_label->setText(m_nCurrentTemString);
}

void Room::_SetWindSpeed(int nWindSpeed_temp){//设置风速
    m_nWindSpeed=nWindSpeed_temp;
    QString s;
    qDebug()<<"设置现在的风速为";
    if(m_nWindSpeed==0){
        qDebug()<<"低风";
        ui->WindSpeed_label->setText("低");
        //CTemPropel->open();//温度推进线程转变为有送风的模式
    }
    else if(m_nWindSpeed==1){
        qDebug()<<"中风";
        ui->WindSpeed_label->setText("中");
        //CTemPropel->open();//温度推进线程转变为有送风的模式
    }
    else if(m_nWindSpeed==2){
        qDebug()<<"高风";
        ui->WindSpeed_label->setText("高");
        //CTemPropel->open();//温度推进线程转变为有送风的模式
    }
    else if(m_nWindSpeed==3){//停止送风，清空风速面板
        qDebug()<<"停";
        ui->WindSpeed_label->clear();
        //CTemPropel->stop();//温度推进线程转变为无送风的模式
    }
}

void Room::_SetUnWindSpeed(int nUnWindSpeed_temp){
    qDebug()<<"未确认风速设置";
    m_nUnWindSpeed=nUnWindSpeed_temp;
}


void Room::_SetElecUsing(double fElecUsing){
    m_fElecUsing=fElecUsing;
    QString s=QString("%1").arg(fElecUsing);
    qDebug()<<"设置花费电量为"<<s;
    ui->ElecUsing_label->setText(s);
}


void Room::_SetMoneyUsing(double fMoneyUsing_temp){
    m_fMoneyUsing=fMoneyUsing_temp;
    QString s=QString("%1").arg(m_fMoneyUsing);
    qDebug()<<"设置花费金额为"<<s;
    ui->MoneyUsing_label->setText(s);
}

void Room::_SetWorkStyle(int nWorkStyle_temp){
    m_nWorkStyle=nWorkStyle_temp;
    qDebug()<<"设置工作模式为"<<nWorkStyle_temp;
    if(m_nWorkStyle==0){
        ui->WorkStyle_label->setText("制冷");
    }
    else if(m_nWorkStyle==1){
        ui->WorkStyle_label->setText("制暖");
    }
}

void Room::_SetSettedTem(int nSettedTem_temp){//设置现在自己的温度设置
    m_nSettedTem=nSettedTem_temp;
   // ui->SettedTem_lcdNumber->setDigitCount(m_nSettedTem);
}

void Room::_SetUnSettedTem(int nUnSettedTem_temp){
    m_nUnSettedTem=nUnSettedTem_temp;
    QString m_nUnSettedTemString=QString("%1").arg(m_nUnSettedTem);
    ui->UnSettedTem_label->setText(m_nUnSettedTemString);
}

Room::~Room()
{
    delete ui;
}

void Room::on_up_btn_clicked()
{
    if(m_nWorkStyle==1){//制热
        if(m_nUnSettedTem<initTem){
            this->_SetUnSettedTem(++m_nUnSettedTem);
            QString s="";
            QString s_temp1=QString::number(m_nUnSettedTem,10);//把int型的未确认温度转变为字符串型
            if(m_nUnSettedTem<=25)
                s=s+"request"+"#2#"+roomID+"#0#"+s_temp1+"#*#;";
            else s=s+"request"+"#2#"+roomID+"#1#"+s_temp1+"#*#;";
            qDebug()<<"温度请求信息已经发送";
            SendFromClient->sendMessage(s);
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
            SendFromClient->sendMessage(s);
        }
    }
}

void Room::on_down_btn_clicked()
{
    if(m_nWorkStyle==0){//制冷
        if(m_nUnSettedTem>initTem){
            this->_SetUnSettedTem(--m_nUnSettedTem);

            QString s="";
            QString s_temp1=QString::number(m_nUnSettedTem,10);//把int型的未确认温度转变为字符串型
            if(m_nUnSettedTem<=25)
                s=s+"request"+"#2#"+roomID+"#0#"+s_temp1+"#*#;";
            else s=s+"request"+"#2#"+roomID+"#1#"+s_temp1+"#*#;";
            qDebug()<<"温度请求信息已经发送";
            SendFromClient->sendMessage(s);
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
            SendFromClient->sendMessage(s);
        }
    }
}

void Room::on_ok_btn_clicked()//温度设置请求发送
{
//    QString s="";
//    QString s_temp1=QString::number(m_nUnSettedTem,10);//把int型的未确认温度转变为字符串型
//    if(m_nUnSettedTem<=25)
//        s=s+"request"+"#2#"+roomID+"#0#"+s_temp1+"#*#;";
//    else s=s+"request"+"#2#"+roomID+"#1#"+s_temp1+"#*#;";
//    qDebug()<<"温度请求信息已经发送";
//    SendFromClient->sendMessage(s);
}

void Room::on_high_btn_clicked()//高速风请求发送
{
    this->_SetUnWindSpeed(2);
    QString s="";
    s=s+"request"+"#3#"+roomID+"#2#"+"*#*#;";
    qDebug()<<"高风速请求信息已经发送";
    SendFromClient->sendMessage(s);
}

void Room::on_middle_btn_clicked()//中速风请求发送
{
    this->_SetUnWindSpeed(1);
    QString s="";
    s=s+"request"+"#3#"+roomID+"#1#"+"*#*#;";
    qDebug()<<"中风速请求信息已经发送";
    SendFromClient->sendMessage(s);
}

void Room::on_low_btn_clicked()//低速风请求发送
{
    this->_SetUnWindSpeed(0);
    QString s="";
    s=s+"request"+"#3#"+roomID+"#0#"+"*#*#;";
    qDebug()<<"低风速请求信息已经发送";
    SendFromClient->sendMessage(s);
}

void Room::on_stop_btn_clicked()//停止送风请求发送
{
    this->_SetUnWindSpeed(3);
    QString s="";
    s=s+"request"+"#3#"+roomID+"#3#"+"*#*#;";
    qDebug()<<"停止送风请求信息已经发送";
    SendFromClient->sendMessage(s);
}

void Room::on_over_btn_clicked()
{
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
      SendFromClient->sendMessage(s);
      g_time->stop();//定时器停止

      tcpSocket->close();//TCP socket 关闭
      m_nWindSpeed=3;//把风速设置为停风
      //CTemPropel->stop();//温度推进线程将改为无连接模式
      //this->close();
    }
}

/* 展示用量 */
void Room::on_report_clicked()
{
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

/* 完成登陆 */
void Room::on_loginBtn_2_clicked()
{
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
}

/* 设置风速 */
void Room::on_windspeed_button_clicked()
{
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

/* 设置模式 */
void Room::on_model_button_clicked()
{
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

/* 设置布局 */
void Room::arrange() {
  // 美化界面
  setWindowTitle("client");
  setWindowFlags(Qt::FramelessWindowHint | windowFlags());


  // 设置鼠标样式
  QCursor cursor ;
  QPixmap pixmap(":/ui/Hand2.tga") ;
  cursor = QCursor(pixmap, 0, 0);
  setCursor(cursor);

  ui->speed_group->setVisible(false);
  ui->login_group->setVisible(false);
  ui->model_group->setVisible(false);
  ui->report_group->setVisible(false);

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

  ui->table->xAxis->setRange(10, 0);
  ui->table->yAxis->setRange(16, 35);
  ui->table->xAxis->setTickLabelColor(QColor(255, 255, 255));
  ui->table->replot();
  ui->again->setText("");
  ui->again->setToolTip("重置折线图");
  ui->again->setStyleSheet("QPushButton{border-image: url(:/window/8.png);}");
  ui->speed_button_2->setText("");
  ui->speed_button_2->setStyleSheet("QPushButton{border-image: url(:/window/ok.png);}");
  ui->login_state->setText("");
  ui->login_state->setDisabled(true);
  ui->login_state->setToolTip("未登录");
  ui->login_state->setStyleSheet("QPushButton{border-image: url(:/window/20.png);}");
  ui->master_button->setText("");
  ui->master_button->setToolTip("开关");
  ui->master_button->setStyleSheet("QPushButton{border-image: url(:/window/switch1.png);}");
  ui->UnSettedTem_label->setText("--");
  ui->set_temp_2->setText("--");
  ui->work_state_2->setText(QString::number(this->m_nCurrentTem));
  ui->work_state->setText("");

  ui->high_btn->hide();
  ui->low_btn->hide();
  ui->stop_btn->hide();
  ui->middle_btn->hide();
  ui->ok_btn->hide();
}

/* 注销并且关闭窗口 */
void Room::on_close_clicked()
{
  QString s="";
  s=s+"request"+"#4#"+roomID+"#*#*#*#;";
  qDebug()<<"关机请求信息已经发送";
  SendFromClient->sendMessage(s);
  g_time->stop();//定时器停止

  tcpSocket->close();//TCP socket 关闭
  m_nWindSpeed=3;//把风速设置为停风
  //CTemPropel->stop();//温度推进线程将改为无连接模式
  //this->close();
  close();
}

/* 开电源 */
void Room::on_master_button_clicked()
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
