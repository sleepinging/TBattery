#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QCloseEvent>

#include "battery.h"
#include "batteryevent.h"
#include "icontool.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    btevt_=new BatteryEvent();
    sti_=new QSystemTrayIcon();
    timer_=new QTimer();
    timer_->setInterval(1000);
    //重复检测电量
    connect(timer_,&QTimer::timeout,this,&Widget::updatebtshow);
    //插拔电池的时候更新
    connect(btevt_,&BatteryEvent::PowerChanged,this,&Widget::updatebtshow);
    //双击显示
    connect(sti_,&QSystemTrayIcon::activated,this,&Widget::onactivetray);
    //在系统托盘显示
    sti_->show();
    timer_->start();
}

Widget::~Widget()
{
    delete ui;
    delete sti_;
    sti_=nullptr;
    delete timer_;
    timer_=nullptr;
}

void Widget::updatebtshow()
{
    Battery::Update();
    showbtinfo();
}

void Widget::showbtinfo()
{
    auto bp=Battery::percent;
    ui->lb_bp->setText(QString::number(bp));
    QString st;
    QColor bk;
    switch(Battery::status){
    case Battery::Status::UNCHARGING:
        st="使用电池";
        bk=qRgba(0xff,0xff,0xff,250);
        break;
    case Battery::Status::CHARGING:
        st="正在充电";
        bk=qRgba(0xaf,0xff,0xaf,250);
        break;
    case Battery::Status::UNKNOW:
        st="未知";
        bk=qRgba(0xaf,0xaf,0xaf,250);
        break;
    }
    ui->lb_st->setText(st);
    st+="\n"+QString::number(bp)+"%";
    sti_->setToolTip(st);
    sti_->setIcon(IconTool::GenIcon(bp,bk));
}

void Widget::closeEvent(QCloseEvent *)
{
    //    event->ignore();
}

void Widget::changeEvent(QEvent *event)
{
    if(event->type()==QEvent::WindowStateChange){
        if(windowState() & Qt::WindowMinimized){
            hide();
        }
    }
    QWidget::changeEvent(event);
}

void Widget::onactivetray(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    // 单击
    case QSystemTrayIcon::Trigger:
        break;
    // 双击
    case QSystemTrayIcon::DoubleClick:
    {
        if(this->isHidden()){
            show();
            if(this->isMinimized()){
                this->showNormal();
            }
        }else{
            this->hide();
        }
        break;
    }
    default:
        break;
    }
}
