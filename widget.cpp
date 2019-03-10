#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QColorDialog>

#include "battery.h"
#include "batteryevent.h"
#include "icontool.h"
#include "config.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    btevt_=new BatteryEvent();
    timer_=new QTimer();
    menu_=new QMenu();

    //初始化托盘
    inittray();

    //重复检测电量
    connect(timer_,&QTimer::timeout,this,&Widget::updatebtshow);
    //插拔电源的时候更新
    connect(btevt_,&BatteryEvent::PowerChanged,this,&Widget::updatebtshow);

    timer_->setInterval(1000);
    timer_->start();
}

Widget::~Widget()
{
    delete ui;
    delete sti_;
    sti_=nullptr;
    delete timer_;
    timer_=nullptr;
    delete menu_;
    menu_=nullptr;
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
    switch(Battery::status){
    case Battery::Status::UNCHARGING:
        st="使用电池";
        break;
    case Battery::Status::CHARGING:
        st="正在充电";
        break;
    case Battery::Status::UNKNOW:
        st="未知";
        break;
    }
    ui->lb_st->setText(st);
    st+="\n"+QString::number(bp)+"%";
    sti_->setToolTip(st);
    sti_->setIcon(IconTool::GenIcon(bp,Battery::status==Battery::Status::CHARGING));
}

void Widget::inittray()
{
    sti_=new QSystemTrayIcon();
    //双击显示
    connect(sti_,&QSystemTrayIcon::activated,this,&Widget::onactivetray);

    //右键菜单
    auto act1 = new QAction(menu_);
    auto act2 = new QAction(menu_);

    act1->setText("显示");
    act2->setText("退出");

    menu_->addAction(act1);
    menu_->addAction(act2);

    connect(act1, &QAction::triggered, this, &Widget::showmain);
    connect(act2, &QAction::triggered, this, &Widget::close);

    sti_->setContextMenu(menu_);

    //在系统托盘显示
    sti_->show();
}

void Widget::showmain()
{
    show();
    if(this->isMinimized()){
        this->showNormal();
    }
}

void Widget::closeEvent(QCloseEvent *e)
{
    auto r=QMessageBox::information(this,"提示","是否退出?",QMessageBox::Yes | QMessageBox::Cancel);
    switch(r){
    case QMessageBox::Yes:
        e->accept();
        break;
    case QMessageBox::Cancel:
        e->ignore();
        break;
    default:
        QWidget::closeEvent(e);
    }
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
            showmain();
        }else{
            this->hide();
        }
        break;
    }
    default:
        break;
    }
}

//充电背景色
void Widget::on_btn_scbc_clicked()
{
    QColorDialog cd;
    auto cl=cd.getColor();
    Config::GetInstance()->color_charging=cl;
    Config::Save("config.ini");
    IconTool::ClearCache();
}

//使用电池的背景色
void Widget::on_btn_subc_clicked()
{
    QColorDialog cd;
    auto cl=cd.getColor();
    Config::GetInstance()->color_us_bt=cl;
    Config::Save("config.ini");
    IconTool::ClearCache();
}
