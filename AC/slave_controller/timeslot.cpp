#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::timeslot()
{
    //更新面板
    //更新登录状态
    if(this->loginState == login_const)
    {
        ui->login_state->setToolTip("连接成功");
        ui->login_state->setStyleSheet("QPushButton{border-image: url(:/window/19.png);}");
    }
    else
    {
        ui->login_state->setToolTip("未登录");
        ui->login_state->setStyleSheet("QPushButton{border-image: url(:/window/20.png);}");
        this->workState = my_idle;
    }

    //更新设定温度
    if(this->loginState == login_const)
        ui->set_temp->setText(QString::number(this->settemp));

    //更新从机的工作状态和模式选择
    if(this->workState == my_idle) {
        ui->work_state->setText("未工作(空闲)");
        ui->set_temp_2->setText("--");
      }
    else if(this->workMode == cool)
    {
        ui->work_state->setText("制冷");
    }
    else
    {
        ui->work_state->setText("供暖");
    }

    if(this->workMode == cool)
        ui->current_choice->setText("当前模式：制冷");
    else
        ui->current_choice->setText("当前模式：供暖");

    //更新室温
    QStringList tmpList = QString::number(this->currentTemp, 10, 1).split(".");
    ui->current_temp_integer->setText(tmpList[0]);
    ui->current_temp_fraction->setText("."+tmpList[1]);

    //更新窗口标题
    if(this->turnOn == turn_on)
        this->setWindowTitle("从控机"+QString::number(this->slave_no)+"-开机");
    else
        this->setWindowTitle("从控机"+QString::number(this->slave_no)+"-关机");

    //更新风速调节标签
    if(this->windSpeed == lowspeed) {
        //ui->speed_choice_label->setText("当前风速：低速风");
        if (ui->speed_group->isHidden())
          ui->low_button->setChecked(true);
        if(this->workState != my_idle) ui->set_temp_2->setText("低速风");
      }
    else if(this->windSpeed == mediumspeed) {
        //ui->speed_choice_label->setText("当前方式：中速风");
        if (ui->speed_group->isHidden())
          ui->medium_button->setChecked(true);
        if(this->workState != my_idle) ui->set_temp_2->setText("中速风");
      }
    else {
        //ui->speed_choice_label->setText("当前风速：高速风");
        if (ui->speed_group->isHidden())
          ui->high_button->setChecked(true);
        if(this->workState != my_idle) ui->set_temp_2->setText("高速风");
      }

    slotCnt++;
    send_message.clear();
    //工作部分
    //发送请求，由于时间槽设置为1秒，所以1秒之内只有最后一次才有效
    if(((fabs(this->currentTemp-this->settemp) > 1.0 && this->workState == my_idle) || this->workState == my_run) && this->loginState == login_const && this->turnOn == turn_on)
    {
        //发送温控请求
        send_message = "request#2#"+QString::number(this->slave_no)+"#"+QString::number(this->workMode)+"#"+QString::number(this->settemp)+"#*#;";
        //发送风速请求
        send_message = send_message+"request#3#"+QString::number(this->slave_no)+"#"+QString::number(this->windSpeed)+"#*#*#;";
    }

    if(this->loginState == login_const && slotCnt-sendCnt >= this->sendDataFrequency)
    {
        qDebug() << "slotcnt:" << slotCnt << "sendcnt:" << sendCnt;
        sendCnt = slotCnt;
        send_message = send_message+"data#1#"+QString::number(this->slave_no)+"#"+QString::number(this->currentTemp, 10, 1)+"#"+QString::number(this->settemp)+"#"+QString::number(this->workMode)+"#;";
    }

    //计算室温
    if(this->workState == my_run && this->loginState == login_const && this->turnOn == turn_on)//正在运行
    {
        this->isRemindMode = 0;
        if(this->workMode == cool)//制冷
        {
            if(this->currentTemp > this->settemp)
            {
                if(this->windSpeed == mediumspeed)
                    this->currentTemp -= 0.04;
                else if(this->windSpeed == lowspeed)
                    this->currentTemp -= 0.03;
                else if(this->windSpeed == highspeed)
                    this->currentTemp -= 0.05;
            }
            else//发送停风请求
            {
                this->workState = my_idle;
                send_message = send_message+"request#4#"+QString::number(this->slave_no)+"#*#*#*#;";
            }
        }
        else if(this->workMode == warm)//供暖
        {
            if(this->currentTemp < this->settemp)
            {
                if(this->windSpeed == mediumspeed)
                    this->currentTemp += 0.04;
                else if(this->windSpeed == lowspeed)
                    this->currentTemp += 0.03;
                else if(this->windSpeed == highspeed)
                    this->currentTemp += 0.05;
            }
            else
            {
                this->workState = my_idle;
                send_message = send_message+"request#4#"+QString::number(this->slave_no)+"#*#*#*#;";
            }
        }

        ui->work_state_2->setText(QString::number((int)(this->currentTemp + 0.5)));
    }
    else if(this->turnOn == turn_on)//空闲
    {
        if(this->workMode == cool && this->currentTemp < defaultTemp_summer)//制冷
            this->currentTemp += 0.05;
        else if(this->workMode == warm && this->currentTemp > defaultTemp_winter)
            this->currentTemp -= 0.05;
    }

    /* 画图画图画图画图 */
    this->counter++;
    if (this->counter == 3) {
        this->tempData.push_back(currentTemp);
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

        ui->work_state_2->setText(QString::number((int)(this->currentTemp + 0.5)));
      }

    if(this->loginState == logout_const || this->turnOn == turn_off)
    {
        ui->set_temp->setText("--");
        return;
    }
    if(this->loginState == login_const)
    {
       sendMessage();
       qDebug() << "发送数据" << send_message;
    }
}


