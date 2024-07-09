#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qtimer.h"//timer
#include "qdatetime.h"//curent time

mainwindow::mainwindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::mainwindow)
{
    ui->setupUi(this);
    timerUpdate1();
    timerUpdate2();
    timerUpdate3();
    restUpdate();
    workUpdate();

    QTimer *timer1 = new QTimer(this);

    connect(timer1,SIGNAL(timeout()),this,SLOT(timerUpdate1()));

    timer1->start(1000);

}

mainwindow::~mainwindow()
{
    delete ui;
}

void mainwindow::timerUpdate1(void)
{
    static QDateTime workStartTime; // 记录切换到工作模式的时间点

    QDateTime currentTime = QDateTime::currentDateTime();
    QString str = currentTime.toString("hh:mm:ss");
    ui->CurrentTime->setText(str);

    if (isWorkMode && !workStartTime.isValid()) {
        // 如果进入工作模式且工作开始时间点还未记录，则记录当前时间点
        workStartTime = currentTime;

        // 在Label_6显示切换成工作模式瞬间的时间，但只显示一次切换时的时间
        QString label6Text = QString("Last Rest Time\n%1").arg(str);
        ui->Label_6->setText(label6Text);

        // 在Label_3显示比Label_6时间晚两小时的时间
        QDateTime timePlusTwoHours = workStartTime.addSecs(2 * 3600); // 2小时 = 2 * 3600秒
        QString strPlusTwoHours = timePlusTwoHours.toString("hh:mm:ss");
        QString label3Text = QString("Next Rest Time\n%1").arg(strPlusTwoHours);
        ui->Label_3->setText(label3Text);
    } else if (!isWorkMode) {
        // 如果不是工作模式，清空工作开始时间点和Label_6、Label_3的显示内容
        workStartTime = QDateTime();
        ui->Label_6->clear();
        ui->Label_3->clear();
    }
    if(!isWorkMode) {
        ui->Label_3->setText("Next Rest Time");
        ui->Label_6->setText("Last Rest Time");
    }
}

void mainwindow::restUpdate(void)
{
    if(isRestMode){
        // 每秒计数器加1
        // 判断是否需要进位到分钟
        if (rest1 == 60){
            rest1 = 0; // 秒归零
            ++rest2; // 分钟加1
        }
        // 判断是否需要进位到小时
        if (rest2 == 60){
            rest2 = 0; // 分钟归零
            ++rest3; // 小时加1
        }
        // 更新 QLabel 显示的文本
        QString timeStr = QString("Total Rest\n%1:%2:%3")
                              .arg(rest3, 2, 10, QChar('0'))
                              .arg(rest2, 2, 10, QChar('0'))
                              .arg(rest1, 2, 10, QChar('0'));
        ui->Label_5->setText(timeStr);
        ++rest1;
    }
}

void mainwindow::workUpdate(void)
{
    if(isWorkMode){
        // 每秒计数器加1
        // 判断是否需要进位到分钟
        if (work1 == 60){
            work1 = 0; // 秒归零
            ++work2; // 分钟加1
        }
        // 判断是否需要进位到小时
        if (work2 == 60){
            work2 = 0; // 分钟归零
            ++work3; // 小时加1
        }
        // 更新 QLabel 显示的文本
        QString timeStr = QString("Total Work\n%1:%2:%3")
                              .arg(work3, 2, 10, QChar('0'))
                              .arg(work2, 2, 10, QChar('0'))
                              .arg(work1, 2, 10, QChar('0'));
        ui->Label_4->setText(timeStr);
        ++work1;
    }
}

void mainwindow::timerUpdate2(void)
{
    // 更新 QLabel 显示的文本
    if (isWorkMode) {
        QString timeStr = QString("Work Remain\n%1:%2:%3")
                              .arg(count3, 2, 10, QChar('0'))
                              .arg(count2, 2, 10, QChar('0'))
                              .arg(count1, 2, 10, QChar('0'));
        ui->Label_1->setText(timeStr);
        --count1;
        if (count1 < 0) {
            count1 = 59; // 秒归零，重新从59开始倒数
            --count2;    // 分钟减1
            if (count2 < 0) {
                count2 = 59; // 分钟归零，重新从59开始倒数
                --count3;    // 小时减1
                if (count3 < 0) {
                    count3 = 0; // 防止显示负数
                    count2 = 0;
                    count1 = 0;
                }
            }
        }
    }
}
void mainwindow::timerUpdate3(void)
{
    if(isRestMode){
        // 每秒计数器加1
        // 判断是否需要进位到分钟
        if (count21 == 60){
            count21 = 0; // 秒归零
            ++count22; // 分钟加1
        }
        // 判断是否需要进位到小时
        if (count22 == 60){
            count22 = 0; // 分钟归零
            ++count23; // 小时加1
        }
        // 更新 QLabel 显示的文本
        QString timeStr = QString("Current Rest\n%1:%2:%3")
                              .arg(count23, 2, 10, QChar('0'))
                              .arg(count22, 2, 10, QChar('0'))
                              .arg(count21, 2, 10, QChar('0'));
        ui->Label_2->setText(timeStr);
        ++count21;
    }
    else{
        // 如果不是休息模式，重置计数器
        count21 = 0;
        count22 = 0;
        count23 = 0;
        QString timeStr = QString("Current Rest\n%1:%2:%3")
                              .arg(count23, 2, 10, QChar('0'))
                              .arg(count22, 2, 10, QChar('0'))
                              .arg(count21, 2, 10, QChar('0'));
        ui->Label_2->setText(timeStr);
    }
}

void mainwindow::on_Work_clicked()
{
    isWorkMode = true;
    isRestMode = false;

    QTimer *timer2 = new QTimer(this);
    connect(timer2,SIGNAL(timeout()),this,SLOT(timerUpdate2()));
    connect(timer2,SIGNAL(timeout()),this,SLOT(workUpdate()));
    timer2->start(1000);
}

void mainwindow::on_Rest_clicked()
{
    isWorkMode = false;
    isRestMode = true;

    QTimer *timer3 = new QTimer(this);
    connect(timer3,SIGNAL(timeout()),this,SLOT(timerUpdate3()));
    connect(timer3,SIGNAL(timeout()),this,SLOT(restUpdate()));
    timer3->start(1000);
}

