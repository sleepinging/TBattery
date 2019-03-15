#include <QChart>
using namespace QtCharts;

#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QLineSeries>
#include <QCategoryAxis>
#include <QDateTimeAxis>
#include <QScatterSeries>

#include "battery.h"
#include "batteryevent.h"
#include "icontool.h"
#include "config.h"
#include "batteryrecord.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    btevt_=new BatteryEvent();
    timer_=new QTimer();
    menu_=new QMenu();
    cld_c_=new QColorDialog();
    cld_b_=new QColorDialog();

    //初始化托盘
    inittray();

    //重复检测电量
    connect(timer_,&QTimer::timeout,this,&Widget::updatebtshow);
    //插拔电源的时候更新
    connect(btevt_,&BatteryEvent::PowerChanged,this,&Widget::updatebtshow);
    connect(btevt_,&BatteryEvent::PowerChanged,this,&Widget::save_record);
    //选择颜色
    connect(cld_c_,&QColorDialog::colorSelected,this,&Widget::selected_bkc_c);
    connect(cld_b_,&QColorDialog::colorSelected,this,&Widget::selected_bkc_b);

    timer_->setInterval(1000);
    timer_->start();
    updatebtshow();

    init_bt_rec();
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
    delete cld_c_;
    cld_c_=nullptr;
    delete cld_b_;
    cld_b_=nullptr;
    delete timer_save_record_;
    timer_save_record_=nullptr;
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

void Widget::init_bt_rec()
{
    BatteryRecord::Init();

    timer_save_record_=new QTimer();
    //保存电量记录
    connect(timer_save_record_,&QTimer::timeout,this,&Widget::save_record);

    timer_save_record_->setInterval(1000*60);
    timer_save_record_->start();
    Widget::save_record();

    //    auto recs=BatteryRecord::GetInstance()->GetRecords(1552541519,1552541885);
    //    for(const auto& p:recs){
    //        qDebug()<<"time:"<<std::get<0>(p)
    //               <<" per:"<<std::get<1>(p)
    //              <<"status:"<<std::get<2>(p);
    //    }
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

void Widget::selected_bkc_c(const QColor &color)
{
    auto cf=Config::GetInstance();
    cf->color_charging=color.rgba();
    cf->Save("config.ini");
    IconTool::ClearCache();
}

void Widget::selected_bkc_b(const QColor &color)
{
    auto cf=Config::GetInstance();
    cf->color_us_bt=color.rgba();
    cf->Save("config.ini");
    IconTool::ClearCache();
}

void Widget::save_record()
{
    BatteryRecord::GetInstance()->AddRecord(
                time(nullptr),
                Battery::percent,
                Battery::status==Battery::Status::CHARGING
                );
}

//充电背景色
void Widget::on_btn_scbc_clicked()
{
    auto cf=Config::GetInstance();
    cld_c_->setCurrentColor(cf->color_charging);
    cld_c_->show();
}

//使用电池的背景色
void Widget::on_btn_subc_clicked()
{
    auto cf=Config::GetInstance();
    cld_b_->setCurrentColor(cf->color_us_bt);
    cld_b_->show();
}

void Widget::on_tabWidget_currentChanged(int index)
{
    //    qDebug()<<index;
    if(index==1){
        on_select_tab_rec(index);
    }
}

void Widget::on_select_tab_rec(int )
{
    auto dt=QDateTime::currentDateTime();
    auto now=dt.toTime_t();
    auto recs=BatteryRecord::GetInstance()->GetRecords(now-86400,now);

    //电量的折线
    QLineSeries* line = new QLineSeries();
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(3);
    pen.setColor(Qt::blue);
    line->setPen(pen);
    //    line->clear();
    // 创建散列点的序列,显示充电点
    QScatterSeries *scatterSeries = new QScatterSeries();
    scatterSeries->setColor(Qt::green);
    scatterSeries->setMarkerSize(5);
    for(const auto& rec:recs){
        line->append(std::get<0>(rec)*1000,std::get<1>(rec));
        if(std::get<2>(rec)){
            scatterSeries->append(std::get<0>(rec)*1000,std::get<1>(rec));
        }
    }
    static QChart* c = new QChart();
    c->legend()->hide();  // 隐藏图例
    for(const auto&s:c->series()){
        c->removeSeries(s);
    }
    for(const auto &a:c->axes()){
        c->removeAxis(a);
    }
    for(const auto &s:line->attachedAxes()){
        line->detachAxis(s);
    }
    for(const auto &s:scatterSeries->attachedAxes()){
        line->detachAxis(s);
    }
    c->addSeries(line);
    c->addSeries(scatterSeries);
    //        c->createDefaultAxes();// 基于已添加到图表的 series 来创轴
    dt.addSecs(3600*-12).toString("HH:mm");

    QDateTimeAxis *axisX = new  QDateTimeAxis;
    axisX->setTickCount(9);
    axisX->setRange(dt.addDays(-1),dt);
    axisX->setFormat("HH:mm");
    c->addAxis(axisX,Qt::AlignBottom);
    line->attachAxis(axisX);
    scatterSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 100);
    axisY->setMin(0);
    axisY->setMax(100);
    axisY->setLabelFormat("%d");
    axisY->setTickCount(11);//10格子
    c->addAxis(axisY,Qt::AlignLeft);
    line->attachAxis(axisY);
    scatterSeries->attachAxis(axisY);

    c->setTitle("24小时的电量记录");  // 设置图表的标题

    ui->widget->setChart(c);
    ui->widget->setRenderHint(QPainter::Antialiasing);    //抗锯齿
}
