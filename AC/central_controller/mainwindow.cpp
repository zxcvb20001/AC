#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "my_server.h"
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIcon exeIcon(":/central_surface.ico");//添加程序的图标
    this->setWindowIcon(exeIcon);
    setWindowTitle("中央空调-未开机");
    setGeometry(200,250,613,365);
    setFixedSize(613,365);

    if (!createConnection())//连接数据库
         qDebug() << "不能连接数据库！";
    //数据库操作
    model = new QSqlTableModel(this);
    model->setTable("reportdata");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); //选取整个表的所有行

    //初始化中央空调设置
    this->setWorkState(idle);
    this->setTurnOn(turnOff);
    this->setSlaveNumber(0);
    this->setSchedule(FCFS);
    this->updateFrequency = 1; //默认刷新频率为1秒
    this->turn = 0; //初始RR的turn为0

    //建立TcpServer
    this->my_tcpserver=new my_server(this);
    //listen();

    QFile sourceData(":/number_password.txt");
    if(!sourceData.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open the file by reading!";
        return;
    }
    QTextStream icin(&sourceData);
    for(int i = 1; i <= 10; i++)
    {
        icin >> this->userInfo[i].slave_no >> this->userInfo[i].password;
        this->userInfo[i].LoginState = Logout_constant;
    }

    //初始化从机队列
    this->head = new slaveNode();
    this->head->next = NULL;

    //初始化中央空调面板
    ui->calendar_widget->setVisible(false);
    ui->report_table_group->setVisible(false);
    ui->config_group->setVisible(false);
    ui->connect_number->setText("0");
    ui->workstate_img->setPixmap(QPixmap(":/cool.png"));
    ui->schedule->setText("先来先到算法");

    //刷新时间槽函数
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeslot()));

    QTimer *timer1 = new QTimer(this);
    timer1->start(1);
    connect(timer1,SIGNAL(timeout()),this,SLOT(DealWithBag()));
}

MainWindow::~MainWindow()
{
    int size = my_tcpserver->socket_list.size();
    for(int i = 0; i < size; i++)
    {
        my_tcpserver->socket_list[i]->sendMessage("instruction#1#*#0#*#*;");
        my_tcpserver->socket_list[i]->deleteLater();
    }
    my_tcpserver->close();
    delete ui;
}

bool MainWindow::createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("userData.db");

    QWidget *win = new QWidget;
    if (!db.open()) {
       QMessageBox::critical(0, win->tr("Cannot open database"),
           win->tr("Unable to establisha database connection."
                     ), QMessageBox::Cancel);
       return false;
    }
    QSqlQuery query;

    //创建数据库，格式如下
    query.exec(QString("create table reportdata(id int, beginTimeStamp vchar(100), endTimeStamp vchar(100), beginTemp float, endTemp float, bill double)"));
    return true;
}

void MainWindow::listen()//侦听
{
    my_tcpserver->listen(QHostAddress::Any,6666);//监听的端口号
    if(!my_tcpserver->listen(QHostAddress::Any,6666))
    {  //**本地主机的6666端口，如果出错就输出错误信息，并关闭
        qDebug() << my_tcpserver->errorString();
        close();
    }
}

void MainWindow::DealWithBag()
{
    int size = my_tcpserver->socket_list.size();
    for(int i = 0; i < size; i++)
    {
        while(!my_tcpserver->socket_list.at(i)->DataBagList.empty())
        {
            qDebug() << "bagbag!!";
            QString message = my_tcpserver->socket_list.at(i)->DataBagList.head();
            if(message == "")//处理无用信息
            {
                my_tcpserver->socket_list.at(i)->DataBagList.dequeue();
                continue;
            }
            QStringList message_List = message.split("#");
            if(message_List[0] == "request")
                 DealWithRequest(message);
            else if(message_List[0] == "data")
                 DealWithData(message);
            else
                 qDebug() << "Bad message from slave controller...";
            my_tcpserver->socket_list.at(i)->DataBagList.dequeue();
        }
    }
}

void MainWindow::DealWithRequest(QString message)
{
    //未开机则不响应任何请求
    if(this->turnOnState == turnOff) return;

    QStringList message_List = message.split("#");
    int slave_no = message_List[2].toInt();
    if(message_List[1] == "1")//连接主机请求
    {
        if(this->userInfo[slave_no].LoginState == Logout_constant)
        {
            QString pass_word = message_List[3];
            if(slave_no > 10 || this->userInfo[slave_no].password != pass_word)
            {
                this->send_message[slave_no].clear();
                this->send_message[slave_no] = "instruction#1#"+message_List[2]+"#0#*#*;";
            }
            else if(this->userInfo[slave_no].password == pass_word)
            {
                this->slave_number++;
                this->addSlave(slave_no);
                this->userInfo[slave_no].LoginState = Login_constant;
                this->send_message[slave_no].clear();
                this->send_message[slave_no] = "instruction#1#"+message_List[2]+"#1#";
                if(this->workState == cool)
                    this->send_message[slave_no] = this->send_message[slave_no]+"0#22;";
                else
                    this->send_message[slave_no] = this->send_message[slave_no]+"1#28;";
                this->send_message[slave_no] = this->send_message[slave_no]+"instruction#4#"+message_List[2]+"#*#"+QString::number(this->updateFrequency, 10, 0)+"#*;";

            }
            int size = this->my_tcpserver->socket_list.size();
            for(int i = 0; i < size; i++)
            {
                my_tcpserver->socket_list.at(i)->sendMessage(this->send_message[slave_no]);
            }
            this->send_message[slave_no].clear();
        }
    }
    else if(message_List[1] == "2" || message_List[1] == "3" || message_List[1] == "4")
    {
        this->updateSlave(message);
    }
    else if(message_List[1] == "5")//注销或者关机请求
    {
        this->slave_number--;
        this->deleteSlave(slave_no);
        this->userInfo[slave_no].LoginState = Logout_constant;
    }
    else
        qDebug() << "Bad message from slave controller...";
}

void MainWindow::DealWithData(QString message)
{
    //未开机则不响应任何请求
    if(this->turnOnState == turnOff) return;

    //更新从控机数据
    QStringList List = message.split("#");
    int no = List[2].toInt();
    if(List[1] == "1")//状态信息
    {
        slaveNode *p = this->head->next;
        while(p && p->slave_no != no) p = p->next;
        p->current_temp = List[3].toDouble();
        p->set_temp = List[4].toInt();
        p->mode = List[5].toInt();

        if(p->startTemp == -1 || p->startTemp == 0)
            p->startTemp = List[3].toDouble();
    }
    else
    {
        //ACK不进行处理;
    }
}

void MainWindow::addSlave(int no)
{
    slaveNode *p = this->head;
    slaveNode *tmp = new slaveNode();
    tmp->slave_no = no;
    tmp->next = NULL;
    
    while(p->next)
    {
        p = p->next;
    }
    p->next = tmp;
    UpdateCombobox();
}

void MainWindow::addDataBase(slaveNode *tmp)
{
    //在数据库增加一条记录
    model->setTable("reportdata");   //重新关联表
    model->select();   //这样才能再次显示整个表的内容
    int row = model->rowCount();
    model->insertRow(row); //添加一行
    QDateTime time_now;
    QString now_longtimestamp = time_now.currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    model->setData(model->index(row,0), tmp->slave_no);
    model->setData(model->index(row,1), tmp->connect_longtimestamp);
    model->setData(model->index(row,2), now_longtimestamp);
    model->setData(model->index(row,3), QString::number(tmp->startTemp, 10, 1).toDouble());
    model->setData(model->index(row,4), QString::number(tmp->current_temp, 10, 1).toDouble());
    model->setData(model->index(row,5), QString::number(tmp->cost, 10, 2).toDouble());
    model->submitAll();

}

void MainWindow::deleteSlave(int no)
{
    slaveNode *p = this->head;
    slaveNode *tmp;
    while(p->next)
    {
        if(p->next->slave_no == no)
        {
            tmp = p->next;
            p->next = p->next->next;
            addDataBase(tmp);
            delete tmp;
            break;
        }
        p = p->next;
    }
    UpdateCombobox();
}

void MainWindow::updateSlave(QString message)
{
    QStringList List = message.split("#");
    int number = List[2].toInt();
    slaveNode *p = this->head->next;
    while(p && p->slave_no != number)
    {
        p = p->next;
    }
    if(List[1] == "2")//温控请求
    {
        p->mode = List[3].toInt();
        p->set_temp = List[4].toInt();
        p->isRequest = 1;
    }
    else if(List[1] == "3")
    {
        p->windspeed = List[3].toInt();
        p->isRequest = 1;
    }
    else if(List[1] == "4")
    {
        p->isRequest = 0;
        p->isWorking = 0;
    }
}

int MainWindow::getTurnOn()
{
    return this->turnOnState;
}

void MainWindow::setTurnOn(int tt)
{
    this->turnOnState = tt;
}

int MainWindow::getWorkState()
{
    return this->workState;
}

void MainWindow::setWorkState(int tt)
{
    this->workState = tt;
}

int MainWindow::getSlaveNumber()
{
    return this->slave_number;
}

void MainWindow::setSlaveNumber(int tt)
{
   this->slave_number = tt;
}

int MainWindow::getSchedule()
{
    return this->schedule;
}

void MainWindow::setSchedule(int tt)
{
    this->schedule = tt;
}

void MainWindow::UpdateCombobox()
{
    //更新从机列表combobox
    ui->slavelist_comboBox->clear();
    slaveNode *p = this->head->next;
    while(p)
    {
        QIcon comboIcon(":/slave_surface.ico");
        ui->slavelist_comboBox->addItem(comboIcon, "从机"+QString::number(p->slave_no));
        p = p->next;
    }
}

void MainWindow::on_start_button_clicked()
{
    int turn_on_state = this->turnOnState;
    QMessageBox *msgBox = new QMessageBox();
    QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
    for(int i = 0; i < textFieldLabels.size(); i++)
    {
        textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
        //设置文字显示的对齐方式
    }
    QIcon exeIcon(":/central_surface.ico");//添加程序的图标
    msgBox->setWindowIcon(exeIcon);
    msgBox->setGeometry(400,350,500,281);
    if(turn_on_state == turnOff)
    {
        my_tcpserver->listen(QHostAddress::Any,6666);
        msgBox->setWindowTitle("开机");
        this->setTurnOn(turnOn);
        this->setWorkState(cool); //默认制冷
        msgBox->setText("成功开机!");
        this->setWindowTitle("中央空调-开机");
        msgBox->addButton("确定", QMessageBox::AcceptRole);
        msgBox->exec();
    }
    else
    {
        QMessageBox *msgBox1 = new QMessageBox();
        QList<QLabel*> textFieldLabels = msgBox1->findChildren<QLabel*>();
        for(int i = 0; i < textFieldLabels.size(); i++)
        {
            textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
            //设置文字显示的对齐方式
        }
        msgBox1->setWindowTitle("通知");
        QIcon exeIcon(":/central_surface.ico");//添加程序的图标
        msgBox1->setWindowIcon(exeIcon);
        msgBox1->setGeometry(400,350,500,281);
        msgBox1->setText("确定要关机？");
        msgBox1->addButton("确定", QMessageBox::AcceptRole);
        msgBox1->addButton("取消", QMessageBox::RejectRole);
        if(msgBox1->exec() == QMessageBox::AcceptRole)
        {
            int size = my_tcpserver->socket_list.size();
            for(int i = 0; i < size; i++)
            {
                my_tcpserver->socket_list[i]->sendMessage("instruction#1#*#0#*#*;");
                my_tcpserver->socket_list[i]->deleteLater();
            }
            my_tcpserver->close();
            msgBox->setWindowTitle("关机");
            this->setTurnOn(turnOff);
            msgBox->setText("已关机!");
            this->setWindowTitle("中央空调-未开机");
            msgBox->addButton("确定", QMessageBox::AcceptRole);
            msgBox->exec();
        }
    }
}

void MainWindow::on_report_button_clicked()
{
    if(this->turnOnState == turnOff)
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
        msgBox->setGeometry(400,350,500,281);
        msgBox->setText("请先开机！");
        msgBox->addButton("确定", QMessageBox::AcceptRole);
        msgBox->exec();
        return;
    }
    if(ui->report_table_group->isVisible() == false)
    {
        ui->report_table_group->setVisible(true);
        ui->report_tableWidget->setStyleSheet("color: rgb(0, 0, 0);background-image: url();background-color: rgb(255, 255, 255, 60%);");
        QPropertyAnimation *pAnimation = new QPropertyAnimation(ui->report_table_group, "geometry");
        QRect geometry = ui->report_table_group->geometry();
        pAnimation->setDuration(100);
        pAnimation->setStartValue(QRect(140, 200, 0, 0));
        pAnimation->setEndValue(QRect(geometry.x(), geometry.y(), geometry.width(), geometry.height()));
        pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        ui->report_table_group->setVisible(false);
    }
}

void MainWindow::on_config_button_clicked()
{
    if(this->turnOnState == turnOff)
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
        msgBox->setGeometry(400,350,500,281);
        msgBox->setText("请先开机！");
        msgBox->addButton("确定", QMessageBox::AcceptRole);
        msgBox->exec();
        return;
    }
    if(ui->config_group->isVisible() == false)
    {
        ui->config_group->setVisible(true);
        QPropertyAnimation *pAnimation = new QPropertyAnimation(ui->config_group, "geometry");
        QRect geometry = ui->config_group->geometry();
        pAnimation->setDuration(100);
        pAnimation->setStartValue(QRect(20, 200, 0, 0));
        pAnimation->setEndValue(QRect(geometry.x(), geometry.y(), geometry.width(), geometry.height()));
        pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        ui->config_group->setVisible(false);
    }
}

void MainWindow::on_save_config_button_clicked()
{
    int check_mode = 0, check_schedule = 0;
    int frequency = ui->update_frequency_spinbox->text().toInt();
    this->updateFrequency = frequency;

    //广播修改刷新频率指令
    if(this->slave_number >= 1)
    {
        for(int i = 1; i <= this->slave_number; i++)
        {
            this->send_message[i].clear();
            this->send_message[i] = "instruction#4#*#*#"+QString::number(frequency)+"#*;";
            int size = this->my_tcpserver->socket_list.size();
            for(int j = 0; j < size; j++)
            {
                if(i == j)
                   my_tcpserver->socket_list.at(j)->sendMessage(this->send_message[i]);
            }
            this->send_message[i].clear();
//            mysocket->sendMessage(mysocket->send_message[i]);
        }
    }

    if(ui->FCFS_button->isChecked())
    {
        this->setSchedule(FCFS);
        check_schedule = 1;
    }
    else if(ui->RR_button->isChecked())
    {
        this->setSchedule(RR);
        check_schedule = 1;
    }
    else if(ui->HPR_button->isChecked())
    {
        this->setSchedule(HPR);
        check_schedule = 1;
    }

    if(ui->cool_button->isChecked())
    {
        this->setWorkState(cool);
        check_mode = 1;
    }
    else if(ui->warm_button->isChecked())
    {
        this->setWorkState(warm);
        check_mode = 1;
    }

    QMessageBox *msgBox = new QMessageBox();
    QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
    for(int i = 0; i < textFieldLabels.size(); i++)
    {
        textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
        //设置文字显示的对齐方式
    }
    QIcon exeIcon(":/central_surface.ico");//添加程序的图标
    msgBox->setWindowTitle("设置");
    msgBox->setWindowIcon(exeIcon);
    msgBox->setGeometry(400,350,500,281);
    if(check_mode == 0 && check_schedule == 0)
    {
        msgBox->setText("请选择工作模式和调度算法！");
    }
    else if(check_mode == 0)
    {
        msgBox->setText("请选择工作模式！");
    }
    else if(check_schedule == 0)
    {
        msgBox->setText("请选择调度算法！");
    }
    else
    {
        msgBox->setText("保存成功！");
        msgBox->addButton("确定", QMessageBox::AcceptRole);
        msgBox->exec();
        ui->config_group->setVisible(false);
        return;
    }
    msgBox->addButton("确定", QMessageBox::AcceptRole);
    msgBox->exec();
}

void MainWindow::on_search_pushButton_clicked()
{
    if(ui->search_lineEdit->text() == "")
    {
        QMessageBox *msgBox = new QMessageBox();
        QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
        for(int i = 0; i < textFieldLabels.size(); i++)
        {
            textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
            //设置文字显示的对齐方式
        }
        QIcon exeIcon(":/central_surface.ico");//添加程序的图标
        msgBox->setWindowIcon(exeIcon);
        msgBox->setGeometry(400,350,500,281);
        msgBox->setWindowTitle("警告");
        msgBox->setText("请输入要查询的从机号!");
        msgBox->addButton("确定", QMessageBox::AcceptRole);
        msgBox->exec();
        return;
    }
    QString date = ui->calendar_widget->selectedDate().toString("yyyy-MM-dd");
    //qDebug() << date;
    if(date == "")
    {
        QMessageBox *msgBox = new QMessageBox();
        QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
        for(int i = 0; i < textFieldLabels.size(); i++)
        {
            textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
            //设置文字显示的对齐方式
        }
        QIcon exeIcon(":/central_surface.ico");//添加程序的图标
        msgBox->setWindowIcon(exeIcon);
        msgBox->setGeometry(400,350,500,281);
        msgBox->setWindowTitle("警告");
        msgBox->setText("请先选择要查询的日期!");
        msgBox->addButton("确定", QMessageBox::AcceptRole);
        msgBox->exec();
        return;
    }
    int number = ui->search_lineEdit->text().toInt();
    ui->report_tableWidget->clearContents();

    model->setTable("reportdata");   //重新关联表
    model->select();   //这样才能再次显示整个表的内容
    model->setFilter(QString("id = '%1'").arg(number));
    model->select();
    int row = model->rowCount();
    int cnt = -1;

    if(ui->daily_report_radioButton->isChecked())//日报表
    {
        for(int i = 0; i < row; i++)
        {
            QStringList tt = model->index(i, 1).data().toString().split(" ");
            if(!QString::compare(date, tt[0]))//找到一条记录
            {
                cnt++;
                for(int j = 0; j <= 5; j++)
                {
                    QTableWidgetItem *temp = new QTableWidgetItem(model->index(i, j).data().toString());
                    ui->report_tableWidget->setItem(cnt, j, temp);
                }
            }
        }
    }
    else if(ui->weekly_report_radioButton->isChecked())//周报表
    {
        QDate time1 = ui->calendar_widget->selectedDate();
        for(int i = 0; i < row; i++)
        {
            QDate time2 = QDateTime::fromString(model->index(i, 1).data().toString(), "yyyy-MM-dd hh:mm:ss").date();
            if(time1.weekNumber() == time2.weekNumber() && time1.year() == time2.year())//找到一条记录
            {
                cnt++;
                for(int j = 0; j <= 5; j++)
                {
                    QTableWidgetItem *temp = new QTableWidgetItem(model->index(i, j).data().toString());
                    ui->report_tableWidget->setItem(cnt, j, temp);
                }
            }
        }
    }
    else if(ui->mouthly_report_radioButton->isChecked())//月报表
    {
        for(int i = 0; i < row; i++)
        {
            QStringList tt1 = date.split("-");
            QStringList tt = model->index(i, 1).data().toString().split(" ");
            QStringList tt2 = tt[0].split("-");
            if(!QString::compare(tt1[0], tt2[0]) && !QString::compare(tt1[1], tt2[1]))//找到一条记录
            {
                cnt++;
                for(int j = 0; j <= 5; j++)
                {
                    QTableWidgetItem *temp = new QTableWidgetItem(model->index(i, j).data().toString());
                    ui->report_tableWidget->setItem(cnt, j, temp);
                }
            }
        }
    }
    else//未选中任何一个报表按钮
    {
        QMessageBox *msgBox = new QMessageBox();
        QList<QLabel*> textFieldLabels = msgBox->findChildren<QLabel*>();
        for(int i = 0; i < textFieldLabels.size(); i++)
        {
            textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
            //设置文字显示的对齐方式
        }
        QIcon exeIcon(":/central_surface.ico");//添加程序的图标
        msgBox->setWindowIcon(exeIcon);
        msgBox->setGeometry(400,350,500,281);
        msgBox->setWindowTitle("警告");
        msgBox->setText("请选择报表模式!");
        msgBox->addButton("确定", QMessageBox::AcceptRole);
        msgBox->exec();
    }
}

void MainWindow::on_config_close_pushButton_clicked()
{
    ui->config_group->setVisible(false);
}

void MainWindow::on_report_table_close_pushButton_clicked()
{
    ui->calendar_widget->setVisible(false);
    ui->report_table_group->setVisible(false);
}

void MainWindow::on_calendar_pushButton_clicked()
{
    if(!ui->calendar_widget->isVisible())
    {
        ui->calendar_widget->setVisible(true);
        QPropertyAnimation *pAnimation = new QPropertyAnimation(ui->calendar_widget, "geometry");
        QRect geometry = ui->calendar_widget->geometry();
        pAnimation->setDuration(100);
        pAnimation->setStartValue(QRect(500, 16, 0, 0));
        pAnimation->setEndValue(QRect(geometry.x(), geometry.y(), geometry.width(), geometry.height()));
        pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        ui->calendar_widget->setVisible(false);
    }
}

void MainWindow::on_clear_db_pushButton_clicked()//清空数据库
{
    QMessageBox *msgBox1 = new QMessageBox();
    QList<QLabel*> textFieldLabels = msgBox1->findChildren<QLabel*>();
    for(int i = 0; i < textFieldLabels.size(); i++)
    {
        textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
        //设置文字显示的对齐方式
    }
    msgBox1->setWindowTitle("通知");
    QIcon exeIcon(":/central_surface.ico");//添加程序的图标
    msgBox1->setWindowIcon(exeIcon);
    msgBox1->setGeometry(400,350,500,281);
    msgBox1->setText("确定要清空数据库？");
    msgBox1->addButton("确定", QMessageBox::AcceptRole);
    msgBox1->addButton("取消", QMessageBox::RejectRole);
    if(msgBox1->exec() == QMessageBox::AcceptRole)
    {
        model->setTable("reportdata");   //重新关联表
        model->select();   //这样才能再次显示整个表的内容
        int row = model->rowCount();    // 获得行数
        for(int i = 0; i < row; i++)//数据库的行是从0开始的
           model->removeRow(i);
        model->submitAll();
    }
}
