/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *logout;
    QLabel *set_label;
    QLabel *current_temp_integer;
    QLabel *set_temp;
    QPushButton *up_temp_button;
    QPushButton *down_temp_button;
    QPushButton *master_button;
    QPushButton *windspeed_button;
    QPushButton *model_button;
    QLabel *login_state_label_2;
    QLabel *work_state;
    QGroupBox *speed_group;
    QRadioButton *low_button;
    QRadioButton *medium_button;
    QRadioButton *high_button;
    QPushButton *speed_button;
    QGroupBox *login_group;
    QLabel *slave_number_label;
    QLineEdit *slave_number_field;
    QLabel *password_label;
    QLineEdit *password_field;
    QPushButton *loginBtn;
    QLabel *current_temp_fraction;
    QGroupBox *report_group;
    QLabel *consumption_label;
    QLabel *bill_label;
    QLabel *consumption;
    QLabel *bill;
    QPushButton *report;
    QPushButton *close;
    QPushButton *min;
    QPushButton *pushButton;
    QGroupBox *model_group;
    QPushButton *cool_button;
    QPushButton *warm_button;
    QLabel *current_choice;
    QPushButton *login_state;
    QLabel *set_label_2;
    QLabel *set_temp_2;
    QPushButton *ccc;
    QLabel *work_state_2;
    QPushButton *ccc_2;
    QCustomPlot *table;
    QPushButton *again;
    QLabel *label;
    QPushButton *pushButton_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(485, 550);
        MainWindow->setStyleSheet(QStringLiteral("background-image: url(:/surface.png);"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        logout = new QPushButton(centralWidget);
        logout->setObjectName(QStringLiteral("logout"));
        logout->setGeometry(QRect(30, 480, 41, 41));
        logout->setCursor(QCursor(Qt::PointingHandCursor));
        logout->setMouseTracking(true);
        logout->setStyleSheet(QLatin1String("color: rgb(0, 0, 0);\n"
"background-image: url();"));
        logout->setFlat(true);
        set_label = new QLabel(centralWidget);
        set_label->setObjectName(QStringLiteral("set_label"));
        set_label->setGeometry(QRect(50, 52, 61, 31));
        set_label->setStyleSheet(QLatin1String("background-image: url();\n"
"font: 13pt \"STHeiti\";\n"
"color: rgb(0, 0, 0);"));
        current_temp_integer = new QLabel(centralWidget);
        current_temp_integer->setObjectName(QStringLiteral("current_temp_integer"));
        current_temp_integer->setGeometry(QRect(45, 152, 67, 71));
        current_temp_integer->setCursor(QCursor(Qt::ArrowCursor));
        current_temp_integer->setStyleSheet(QLatin1String("background-image: url();\n"
"font: italic 36pt \"Verdana\";\n"
"color: rgb(255, 255, 255);"));
        set_temp = new QLabel(centralWidget);
        set_temp->setObjectName(QStringLiteral("set_temp"));
        set_temp->setGeometry(QRect(100, 80, 101, 41));
        set_temp->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"font: 75 24pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(0,0,0);"));
        up_temp_button = new QPushButton(centralWidget);
        up_temp_button->setObjectName(QStringLiteral("up_temp_button"));
        up_temp_button->setGeometry(QRect(230, 420, 50, 50));
        up_temp_button->setCursor(QCursor(Qt::PointingHandCursor));
        up_temp_button->setMouseTracking(true);
        up_temp_button->setStyleSheet(QStringLiteral("background-image: url();"));
        up_temp_button->setFlat(true);
        down_temp_button = new QPushButton(centralWidget);
        down_temp_button->setObjectName(QStringLiteral("down_temp_button"));
        down_temp_button->setGeometry(QRect(230, 470, 50, 50));
        down_temp_button->setCursor(QCursor(Qt::PointingHandCursor));
        down_temp_button->setStyleSheet(QStringLiteral("background-image: url();"));
        down_temp_button->setFlat(true);
        master_button = new QPushButton(centralWidget);
        master_button->setObjectName(QStringLiteral("master_button"));
        master_button->setGeometry(QRect(105, 450, 91, 51));
        master_button->setCursor(QCursor(Qt::PointingHandCursor));
        master_button->setStyleSheet(QStringLiteral("background-image: url();"));
        master_button->setFlat(true);
        windspeed_button = new QPushButton(centralWidget);
        windspeed_button->setObjectName(QStringLiteral("windspeed_button"));
        windspeed_button->setGeometry(QRect(325, 480, 131, 43));
        windspeed_button->setCursor(QCursor(Qt::PointingHandCursor));
        windspeed_button->setMouseTracking(true);
        windspeed_button->setStyleSheet(QStringLiteral("background-image: url();"));
        windspeed_button->setFlat(true);
        model_button = new QPushButton(centralWidget);
        model_button->setObjectName(QStringLiteral("model_button"));
        model_button->setGeometry(QRect(325, 410, 131, 43));
        model_button->setCursor(QCursor(Qt::PointingHandCursor));
        model_button->setStyleSheet(QStringLiteral("background-image: url();"));
        model_button->setFlat(true);
        login_state_label_2 = new QLabel(centralWidget);
        login_state_label_2->setObjectName(QStringLiteral("login_state_label_2"));
        login_state_label_2->setGeometry(QRect(250, 52, 81, 21));
        login_state_label_2->setStyleSheet(QLatin1String("background-image: url();\n"
"color: rgb(0, 0, 0);\n"
"font: 13pt \"STHeiti\";"));
        work_state = new QLabel(centralWidget);
        work_state->setObjectName(QStringLiteral("work_state"));
        work_state->setGeometry(QRect(310, 120, 151, 41));
        work_state->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"color: rgb(0,0,0);\n"
"font: 75 24pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        speed_group = new QGroupBox(centralWidget);
        speed_group->setObjectName(QStringLiteral("speed_group"));
        speed_group->setGeometry(QRect(270, 274, 181, 141));
        speed_group->setStyleSheet(QLatin1String("color: rgb(0, 0, 0);\n"
"font: 11pt \"STHeiti\";\n"
"background-image:url();\n"
"background-color: rgb(85, 85, 255, 0%);"));
        low_button = new QRadioButton(speed_group);
        low_button->setObjectName(QStringLiteral("low_button"));
        low_button->setGeometry(QRect(50, 22, 61, 20));
        low_button->setStyleSheet(QLatin1String("color:rgb(0, 0, 0);\n"
"font: 12pt \"STHeiti\";"));
        medium_button = new QRadioButton(speed_group);
        medium_button->setObjectName(QStringLiteral("medium_button"));
        medium_button->setGeometry(QRect(50, 47, 61, 20));
        medium_button->setStyleSheet(QLatin1String("color:rgb(0, 0, 0);\n"
"font: 12pt \"STHeiti\";"));
        high_button = new QRadioButton(speed_group);
        high_button->setObjectName(QStringLiteral("high_button"));
        high_button->setGeometry(QRect(50, 72, 61, 20));
        high_button->setStyleSheet(QLatin1String("color:rgb(0, 0, 0);\n"
"font: 12pt \"STHeiti\";"));
        speed_button = new QPushButton(speed_group);
        speed_button->setObjectName(QStringLiteral("speed_button"));
        speed_button->setGeometry(QRect(55, 100, 50, 23));
        speed_button->setCursor(QCursor(Qt::PointingHandCursor));
        speed_button->setStyleSheet(QLatin1String("color: rgb(0, 0, 127);\n"
"background-image: url();\n"
"background-color: rgb(225, 225, 225);"));
        login_group = new QGroupBox(centralWidget);
        login_group->setObjectName(QStringLiteral("login_group"));
        login_group->setGeometry(QRect(90, 150, 300, 161));
        login_group->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        slave_number_label = new QLabel(login_group);
        slave_number_label->setObjectName(QStringLiteral("slave_number_label"));
        slave_number_label->setGeometry(QRect(50, 70, 54, 21));
        slave_number_label->setStyleSheet(QLatin1String("background-image: url();\n"
"font: 13pt \"STHeiti\";"));
        slave_number_label->setAlignment(Qt::AlignCenter);
        slave_number_field = new QLineEdit(login_group);
        slave_number_field->setObjectName(QStringLiteral("slave_number_field"));
        slave_number_field->setGeometry(QRect(110, 70, 121, 20));
        slave_number_field->setStyleSheet(QLatin1String("background-image: url();\n"
"color: rgb(0, 0, 0);"));
        password_label = new QLabel(login_group);
        password_label->setObjectName(QStringLiteral("password_label"));
        password_label->setGeometry(QRect(50, 110, 54, 16));
        password_label->setStyleSheet(QLatin1String("background-image: url();\n"
"font: 13pt \"STHeiti\";"));
        password_label->setAlignment(Qt::AlignCenter);
        password_field = new QLineEdit(login_group);
        password_field->setObjectName(QStringLiteral("password_field"));
        password_field->setGeometry(QRect(110, 110, 93, 20));
        password_field->setStyleSheet(QLatin1String("background-image: url();\n"
"color: rgb(0, 0, 0);"));
        loginBtn = new QPushButton(login_group);
        loginBtn->setObjectName(QStringLiteral("loginBtn"));
        loginBtn->setGeometry(QRect(207, 109, 24, 24));
        loginBtn->setCursor(QCursor(Qt::PointingHandCursor));
        current_temp_fraction = new QLabel(centralWidget);
        current_temp_fraction->setObjectName(QStringLiteral("current_temp_fraction"));
        current_temp_fraction->setGeometry(QRect(104, 181, 28, 31));
        current_temp_fraction->setStyleSheet(QLatin1String("background-image: url();\n"
"font: italic 20pt \"Verdana\";\n"
"color: rgb(255, 255, 255);"));
        report_group = new QGroupBox(centralWidget);
        report_group->setObjectName(QStringLiteral("report_group"));
        report_group->setGeometry(QRect(70, 312, 171, 91));
        report_group->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"background-image:url();\n"
"background-color: rgb(0, 170, 255,0%);"));
        consumption_label = new QLabel(report_group);
        consumption_label->setObjectName(QStringLiteral("consumption_label"));
        consumption_label->setGeometry(QRect(10, 30, 51, 21));
        consumption_label->setStyleSheet(QLatin1String("color: rgb(0, 0, 0);\n"
"font: 13pt \"STHeiti\";"));
        bill_label = new QLabel(report_group);
        bill_label->setObjectName(QStringLiteral("bill_label"));
        bill_label->setGeometry(QRect(10, 60, 51, 21));
        bill_label->setStyleSheet(QLatin1String("color:rgb(0, 0, 0);\n"
"font: 13pt \"STHeiti\";"));
        consumption = new QLabel(report_group);
        consumption->setObjectName(QStringLiteral("consumption"));
        consumption->setGeometry(QRect(70, 30, 41, 21));
        consumption->setStyleSheet(QStringLiteral("background-color: transparent;"));
        bill = new QLabel(report_group);
        bill->setObjectName(QStringLiteral("bill"));
        bill->setGeometry(QRect(70, 60, 41, 21));
        bill->setStyleSheet(QStringLiteral("background-color: transparent;"));
        report = new QPushButton(centralWidget);
        report->setObjectName(QStringLiteral("report"));
        report->setGeometry(QRect(30, 420, 41, 41));
        report->setCursor(QCursor(Qt::PointingHandCursor));
        report->setMouseTracking(true);
        report->setStyleSheet(QLatin1String("background-image: url();\n"
"color: rgb(0, 0, 0);"));
        report->setFlat(true);
        close = new QPushButton(centralWidget);
        close->setObjectName(QStringLiteral("close"));
        close->setGeometry(QRect(0, 0, 37, 37));
        close->setContextMenuPolicy(Qt::DefaultContextMenu);
        min = new QPushButton(centralWidget);
        min->setObjectName(QStringLiteral("min"));
        min->setGeometry(QRect(34, 0, 37, 37));
        min->setContextMenuPolicy(Qt::DefaultContextMenu);
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(-10, 37, 650, 750));
        model_group = new QGroupBox(centralWidget);
        model_group->setObjectName(QStringLiteral("model_group"));
        model_group->setGeometry(QRect(250, 286, 191, 121));
        model_group->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"background-image:url();\n"
"background-color: rgb(170, 0, 127, 0%);"));
        cool_button = new QPushButton(model_group);
        cool_button->setObjectName(QStringLiteral("cool_button"));
        cool_button->setGeometry(QRect(30, 30, 48, 48));
        cool_button->setCursor(QCursor(Qt::PointingHandCursor));
        cool_button->setMouseTracking(true);
        cool_button->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        cool_button->setFlat(true);
        warm_button = new QPushButton(model_group);
        warm_button->setObjectName(QStringLiteral("warm_button"));
        warm_button->setGeometry(QRect(110, 30, 48, 48));
        warm_button->setCursor(QCursor(Qt::PointingHandCursor));
        warm_button->setMouseTracking(true);
        warm_button->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        warm_button->setFlat(true);
        current_choice = new QLabel(model_group);
        current_choice->setObjectName(QStringLiteral("current_choice"));
        current_choice->setGeometry(QRect(40, 90, 91, 21));
        current_choice->setStyleSheet(QLatin1String("color:rgb(0, 0, 0);\n"
"font: 12pt \"STHeiti\";"));
        warm_button->raise();
        cool_button->raise();
        current_choice->raise();
        login_state = new QPushButton(centralWidget);
        login_state->setObjectName(QStringLiteral("login_state"));
        login_state->setGeometry(QRect(460, 5, 19, 28));
        set_label_2 = new QLabel(centralWidget);
        set_label_2->setObjectName(QStringLiteral("set_label_2"));
        set_label_2->setGeometry(QRect(50, 120, 61, 31));
        set_label_2->setStyleSheet(QLatin1String("background-image: url();\n"
"font: 13pt \"STHeiti\";\n"
"color: rgb(0, 0, 0);"));
        set_temp_2 = new QLabel(centralWidget);
        set_temp_2->setObjectName(QStringLiteral("set_temp_2"));
        set_temp_2->setGeometry(QRect(100, 150, 101, 41));
        set_temp_2->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"font: 75 24pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(0,0,0);"));
        ccc = new QPushButton(centralWidget);
        ccc->setObjectName(QStringLiteral("ccc"));
        ccc->setGeometry(QRect(150, 85, 30, 32));
        work_state_2 = new QLabel(centralWidget);
        work_state_2->setObjectName(QStringLiteral("work_state_2"));
        work_state_2->setGeometry(QRect(310, 80, 151, 41));
        work_state_2->setStyleSheet(QString::fromUtf8("background-image: url();\n"
"color: rgb(0,0,0);\n"
"font: 75 24pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        ccc_2 = new QPushButton(centralWidget);
        ccc_2->setObjectName(QStringLiteral("ccc_2"));
        ccc_2->setGeometry(QRect(370, 85, 30, 32));
        table = new QCustomPlot(centralWidget);
        table->setObjectName(QStringLiteral("table"));
        table->setGeometry(QRect(20, 190, 441, 201));
        again = new QPushButton(centralWidget);
        again->setObjectName(QStringLiteral("again"));
        again->setGeometry(QRect(410, 325, 32, 32));
        again->setCursor(QCursor(Qt::PointingHandCursor));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 175, 60, 16));
        label->setStyleSheet(QStringLiteral("font: 57 13pt \"STHeiti\";"));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(-10, -10, 650, 60));
        pushButton_2->setStyleSheet(QLatin1String("color:rgb(255, 102, 102);\n"
"background-color: rgb(102, 102, 102 ,100%);"));
        MainWindow->setCentralWidget(centralWidget);
        pushButton_2->raise();
        pushButton->raise();
        table->raise();
        current_temp_integer->raise();
        logout->raise();
        up_temp_button->raise();
        down_temp_button->raise();
        windspeed_button->raise();
        model_button->raise();
        current_temp_fraction->raise();
        report->raise();
        report_group->raise();
        master_button->raise();
        close->raise();
        min->raise();
        login_state_label_2->raise();
        set_label->raise();
        set_temp->raise();
        work_state->raise();
        login_state->raise();
        set_label_2->raise();
        set_temp_2->raise();
        ccc->raise();
        work_state_2->raise();
        ccc_2->raise();
        login_group->raise();
        speed_group->raise();
        model_group->raise();
        again->raise();
        label->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        logout->setToolTip(QApplication::translate("MainWindow", "\347\231\273\345\275\225", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        logout->setText(QApplication::translate("MainWindow", "\347\231\273\345\275\225", Q_NULLPTR));
        set_label->setText(QApplication::translate("MainWindow", "\350\256\276\345\256\232\346\270\251\345\272\246\357\274\232", Q_NULLPTR));
        current_temp_integer->setText(QString());
        set_temp->setText(QApplication::translate("MainWindow", "\350\256\276\345\256\232\344\277\241\346\201\257", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        up_temp_button->setToolTip(QApplication::translate("MainWindow", "\346\217\220\351\253\230\347\233\256\346\240\207\346\270\251\345\272\246", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        up_temp_button->setText(QApplication::translate("MainWindow", "\345\215\207\346\270\251", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        down_temp_button->setToolTip(QApplication::translate("MainWindow", "\351\231\215\344\275\216\350\256\276\347\275\256\346\270\251\345\272\246", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        down_temp_button->setText(QApplication::translate("MainWindow", "\351\231\215\346\270\251", Q_NULLPTR));
        master_button->setText(QApplication::translate("MainWindow", "\345\274\200\346\234\272", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        windspeed_button->setToolTip(QApplication::translate("MainWindow", "\351\231\215\344\275\216\347\233\256\346\240\207\346\270\251\345\272\246", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        windspeed_button->setText(QApplication::translate("MainWindow", "\351\243\216\351\200\237", Q_NULLPTR));
        model_button->setText(QApplication::translate("MainWindow", "\346\250\241\345\274\217", Q_NULLPTR));
        login_state_label_2->setText(QApplication::translate("MainWindow", "\345\256\244\346\270\251\344\270\216\351\243\216\351\200\237\357\274\232", Q_NULLPTR));
        work_state->setText(QApplication::translate("MainWindow", "\345\267\245\344\275\234\347\212\266\346\200\201\344\277\241\346\201\257", Q_NULLPTR));
        speed_group->setTitle(QApplication::translate("MainWindow", "\344\270\211\347\247\215\351\243\216\351\200\237\351\200\211\346\213\251", Q_NULLPTR));
        low_button->setText(QApplication::translate("MainWindow", "\344\275\216\351\200\237\351\243\216", Q_NULLPTR));
        medium_button->setText(QApplication::translate("MainWindow", "\344\270\255\351\200\237\351\243\216", Q_NULLPTR));
        high_button->setText(QApplication::translate("MainWindow", "\351\253\230\351\200\237\351\243\216", Q_NULLPTR));
        speed_button->setText(QApplication::translate("MainWindow", "Ok", Q_NULLPTR));
        login_group->setTitle(QApplication::translate("MainWindow", "\347\224\250\346\210\267\347\231\273\345\275\225", Q_NULLPTR));
        slave_number_label->setText(QApplication::translate("MainWindow", "\344\273\216\346\234\272\345\217\267", Q_NULLPTR));
        password_label->setText(QApplication::translate("MainWindow", "\345\257\206\347\240\201", Q_NULLPTR));
        loginBtn->setText(QApplication::translate("MainWindow", "next", Q_NULLPTR));
        current_temp_fraction->setText(QString());
        report_group->setTitle(QApplication::translate("MainWindow", "\347\224\250\351\207\217\347\273\237\350\256\241", Q_NULLPTR));
        consumption_label->setText(QApplication::translate("MainWindow", "\347\224\250\351\207\217(\347\223\246)\357\274\232", Q_NULLPTR));
        bill_label->setText(QApplication::translate("MainWindow", "\351\207\221\351\242\235(\345\205\203)\357\274\232", Q_NULLPTR));
        consumption->setText(QString());
        bill->setText(QString());
#ifndef QT_NO_TOOLTIP
        report->setToolTip(QApplication::translate("MainWindow", "\346\212\245\350\241\250", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        report->setText(QApplication::translate("MainWindow", "\346\212\245\350\241\250", Q_NULLPTR));
        close->setText(QApplication::translate("MainWindow", "close", Q_NULLPTR));
        min->setText(QApplication::translate("MainWindow", "min", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        model_group->setTitle(QString());
#ifndef QT_NO_TOOLTIP
        cool_button->setToolTip(QApplication::translate("MainWindow", "\351\200\211\346\213\251\345\210\266\345\206\267", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        cool_button->setText(QString());
#ifndef QT_NO_TOOLTIP
        warm_button->setToolTip(QApplication::translate("MainWindow", "\351\200\211\346\213\251\344\276\233\346\232\226", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        warm_button->setText(QString());
        current_choice->setText(QApplication::translate("MainWindow", "\345\275\223\345\211\215\346\250\241\345\274\217\357\274\232", Q_NULLPTR));
        login_state->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        set_label_2->setText(QApplication::translate("MainWindow", "\350\256\276\345\256\232\351\243\216\351\200\237\357\274\232", Q_NULLPTR));
        set_temp_2->setText(QApplication::translate("MainWindow", "\350\256\276\345\256\232\344\277\241\346\201\257", Q_NULLPTR));
        ccc->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        work_state_2->setText(QApplication::translate("MainWindow", "\345\267\245\344\275\234\347\212\266\346\200\201\344\277\241\346\201\257", Q_NULLPTR));
        ccc_2->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        again->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\345\256\236\346\227\266\346\270\251\345\272\246", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
