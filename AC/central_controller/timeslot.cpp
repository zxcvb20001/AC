#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpServer>

void MainWindow::timeslot()
{
    //刷新面板
    //更新工作模式
    if(this->workState == cool)
       ui->workstate_img->setPixmap(QPixmap(":/cool.png"));
    else if(this->workState == warm)
       ui->workstate_img->setPixmap(QPixmap(":/warm.png"));

    //更新连接从机数
    ui->connect_number->setText(QString::number(this->slave_number));

    //更新当前调度算法
    if(this->schedule == FCFS)
        ui->schedule->setText("先来先到算法");
    else if(this->schedule == RR)
        ui->schedule->setText("时间片轮转算法");
    else
        ui->schedule->setText("高速风优先算法");

    //刷新其他数据前提判定
    if(this->workState == idle || this->turnOnState == turnOff)
    {
        this->my_tcpserver->close();
        return;
    }
    int size = my_tcpserver->socket_list.size();
    for(int i = 0; i < size; i++)//如果有从机已经断开连接
    {
        if(!my_tcpserver->socket_list[i]->tcpSocket->isOpen())
        {
            my_tcpserver->socket_list.removeAt(i);//删除该socket
        }
    }
    for(int i = 1; i <= this->slave_number; i++)//清除发送信息数组
        this->send_message[i].clear();

    //调度算法
    switch(this->schedule)
    {
        case FCFS://先来先服务算法
        {
            int cnt = 0;
            slaveNode *p = this->head->next;
            while(p)
            {
                p->connect_duration = p->connect_duration.addSecs(1);
                if(p->mode != this->workState) //不符合主机工作模式
                {
                    qDebug() << p->mode << this->workState;
                    p->isWorking = 0;
                    this->send_message[p->slave_no] = this->send_message[p->slave_no]+"instruction#2#"+QString::number(p->slave_no)+"#0#*#*;";
                }
                else if(cnt < 3 && p->isRequest == 1)
                {
                    cnt++;
                    p->isWorking = 1;
                    if(p->windspeed == lowspeed)
                    {
                        p->power_consumption += 0.8/60;
                        p->cost += 0.8*5/60;
                    }
                    else if(p->windspeed == mediumspeed)
                    {
                        p->power_consumption += 1.0/60;
                        p->cost += 1.0*5/60;
                    }
                    else
                    {
                        p->power_consumption += 1.3/60;
                        p->cost += 1.3*5/60;
                    }
                    this->send_message[p->slave_no] = this->send_message[p->slave_no]+"instruction#2#"+QString::number(p->slave_no)+"#1#*#*;";
                }
                else if(p->isRequest == 1)//没有调度上
                {
                    p->isWorking = 0;
                    this->send_message[p->slave_no] = this->send_message[p->slave_no]+"instruction#3#"+QString::number(p->slave_no)+"#0#*#*;";
                }
                p = p->next;
            }
            break;
        }
        case RR://时间片轮转算法
        {
            int tturn = this->turn;
            int cnt = 0, num = 0;
            slaveNode *p = this->head->next;
            while(tturn > 0)
            {
                tturn--;
                p = p->next;
            }
            while(num < this->slave_number)
            {
                qDebug() << "num:" << num;
                num++;
                p->connect_duration = p->connect_duration.addSecs(1);
                if(p->mode != this->workState) //不符合主机工作模式
                {
                    qDebug() << p->mode << this->workState;
                    p->isWorking = 0;
                    this->send_message[p->slave_no] = this->send_message[p->slave_no]+"instruction#2#"+QString::number(p->slave_no)+"#0#*#*;";
                }
                else if(cnt < 3 && p->isRequest == 1)
                {
                    cnt++;
                    p->isWorking = 1;
                    if(p->windspeed == lowspeed)
                    {
                        p->power_consumption += 0.8/60;
                        p->cost += 0.8*5/60;
                    }
                    else if(p->windspeed == mediumspeed)
                    {
                        p->power_consumption += 1.0/60;
                        p->cost += 1.0*5/60;
                    }
                    else
                    {
                        p->power_consumption += 1.3/60;
                        p->cost += 1.3*5/60;
                    }
                    this->send_message[p->slave_no] = this->send_message[p->slave_no]+"instruction#2#"+QString::number(p->slave_no)+"#1#*#*;";
                }
                else if(p->isRequest == 1)//没有调度上
                {
                    p->isWorking = 0;
                    this->send_message[p->slave_no] = this->send_message[p->slave_no]+"instruction#3#"+QString::number(p->slave_no)+"#0#*#*;";
                }
                p = p->next;
                if(p == NULL)
                    p = this->head->next;
            }
            this->turn = (this->turn+1)%this->slave_number;
            break;
        }
        case HPR://高速风优先算法
        {
            int cnt = 0;
            int speed = highspeed;
            slaveNode *p = this->head->next;
            while(p)//清空工作状态
            {
                p->isWorking = 0;
                p = p->next;
            }
            while(cnt < 3 && speed >= 0)
            {
                p = this->head->next;
                while(p)
                {
                    p->connect_duration = p->connect_duration.addSecs(1);
                    if(cnt < 3 && p->isRequest == 1 && p->windspeed == speed && p->isWorking == 0)
                    {
                        cnt++;
                        p->isWorking = 1;
                        if(p->windspeed == lowspeed)
                        {
                            p->power_consumption += 0.8/60;
                            p->cost += 0.8*5/60;
                        }
                        else if(p->windspeed == mediumspeed)
                        {
                            p->power_consumption += 1.0/60;
                            p->cost += 1.0*5/60;
                        }
                        else
                        {
                            p->power_consumption += 1.3/60;
                            p->cost += 1.3*5/60;
                        }
                    }
                    p = p->next;
                }
                speed--;//按风速高低递减
            }
            p = this->head->next;
            while(p)
            {
                if(p->mode != this->workState) //不符合主机工作模式
                {
                    this->send_message[p->slave_no] = this->send_message[p->slave_no]+"instruction#2#"+QString::number(p->slave_no)+"#0#*#*;";
                }
                else if(p->isWorking == 1) //调度上了
                {
                    this->send_message[p->slave_no] = this->send_message[p->slave_no]+"instruction#2#"+QString::number(p->slave_no)+"#1#*#*;";
                }
                else//没有调度上
                {
                    this->send_message[p->slave_no] = this->send_message[p->slave_no]+"instruction#3#"+QString::number(p->slave_no)+"#0#*#*;";
                }
                p = p->next;
            }
            break;
        }
    }

    //给从机发送报表信息
    slaveNode *p = this->head->next;
    while(p)
    {
        this->send_message[p->slave_no] = this->send_message[p->slave_no]+"information#1#"+QString::number(p->slave_no)+"#"+QString::number(p->power_consumption, 10, 2)+"#"+QString::number(p->cost, 10, 2)+";";
        int size = this->my_tcpserver->socket_list.size();
        qDebug() << "size" << size;
        for(int i = 0; i < size; i++)
        {
            my_tcpserver->socket_list.at(i)->sendMessage(this->send_message[p->slave_no]);
        }
        this->send_message[p->slave_no].clear();
        p = p->next;
    }

    //没有从机，清空面板
    if(this->slave_number == 0)
    {
        ui->isWorking_label->setText("");
        ui->slave_number->setText("");
        ui->set_temp->setText("");
        ui->current_temp->setText("");
        ui->wind_speed->setText("");
        ui->connect_time->setText("");
        ui->connect_duration->setText("");
        ui->power_consumption->setText("");
        ui->bill->setText("");
        return;
    }

    //如果有从机，更新从机面板数据
    QStringList List = ui->slavelist_comboBox->currentText().split("机");
    int show_number = List[1].toInt();
    p = this->head->next;
    while(p && p->slave_no != show_number)
       p = p->next;
    if(p->isWorking == 1)
        ui->isWorking_label->setText("工作中");
    else
        ui->isWorking_label->setText("空闲");
    ui->slave_number->setText(QString::number(p->slave_no));
    ui->set_temp->setText(QString::number(p->set_temp));
    ui->current_temp->setText(QString::number(p->current_temp, 10, 1));
    if(p->windspeed == lowspeed)
       ui->wind_speed->setText("低速风");
    else if(p->windspeed == mediumspeed)
        ui->wind_speed->setText("中速风");
    else if(p->windspeed == highspeed)
        ui->wind_speed->setText("高速风");
    ui->connect_time->setText(p->connect_timestamp);
    ui->connect_duration->setText(p->connect_duration.toString("hh:mm:ss"));
    ui->power_consumption->setText(QString::number(p->power_consumption, 10, 2));
    ui->bill->setText(QString::number(p->cost, 10, 2));

}
