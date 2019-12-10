#include <QChart>
using namespace QtCharts;

#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QCloseEvent>
#include <QTimer>
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
    timer_update_battery_=new QTimer();
    menu_=new QMenu();

    cld_c_=new QColorDialog();
    cld_c_->setOption(QColorDialog::ShowAlphaChannel);
    cld_b_=new QColorDialog();
    cld_b_->setOption(QColorDialog::ShowAlphaChannel);
    cld_f_u_=new QColorDialog();
    cld_f_u_->setOption(QColorDialog::ShowAlphaChannel);
    cld_f_c_=new QColorDialog();
    cld_f_c_->setOption(QColorDialog::ShowAlphaChannel);

    //初始化托盘
    inittray();

    //初始化chart
    init_chart();

    //重复检测电量
    connect(timer_update_battery_,&QTimer::timeout,this,&Widget::updatebtshow);
    //插拔电源的时候更新
    connect(btevt_,&BatteryEvent::PowerChanged,this,&Widget::updatebtshow);
    connect(btevt_,&BatteryEvent::PowerChanged,this,&Widget::save_record);
    //选择颜色
    connect(cld_c_,&QColorDialog::colorSelected,this,&Widget::selected_bkc_c);
    connect(cld_b_,&QColorDialog::colorSelected,this,&Widget::selected_bkc_b);
    connect(cld_f_u_,&QColorDialog::colorSelected,this,&Widget::selected_fc_u);
    connect(cld_f_c_,&QColorDialog::colorSelected,this,&Widget::selected_fc_c);

    timer_update_battery_->setInterval(1000);
    timer_update_battery_->start();
    updatebtshow();

    init_bt_rec();

    timer_update_chart_=new QTimer();
    timer_update_chart_->setInterval(1000*10);
    timer_update_chart_->start();
    connect(timer_update_chart_,&QTimer::timeout,this,&Widget::update_chart_need);
}

Widget::~Widget()
{
    delete ui;

    delete sti_;
    sti_=nullptr;

    delete timer_update_battery_;
    timer_update_battery_=nullptr;

    delete timer_update_chart_;
    timer_update_chart_=nullptr;

    delete menu_;
    menu_=nullptr;

    delete cld_c_;
    cld_c_=nullptr;

    delete cld_b_;
    cld_b_=nullptr;

    delete cld_f_u_;
    cld_f_u_=nullptr;

    delete cld_f_c_;
    cld_f_c_=nullptr;

    delete timer_save_record_;
    timer_save_record_=nullptr;

    delete chart_;
    chart_=nullptr;
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

void Widget::init_chart()
{
    chart_ = new QtCharts::QChart();
    chart_->legend()->hide();  // 隐藏图例
    chart_->setTitle("24小时的电量记录");  // 设置图表的标题
    ui->widget->setChart(chart_);
    ui->widget->setRenderHint(QPainter::Antialiasing);    //抗锯齿
}

void Widget::showmain()
{
    show();
    if(this->isMinimized()){
        this->showNormal();
    }
}

void Widget::update_chart_need()
{
    auto idx=ui->tabWidget->currentIndex();
//    qDebug()<<"idx:"<<idx;
    if(idx != 1){
        return;
    }
    if(this->isMinimized()){
        return;
    }
//    qDebug()<<"update chart";
    update_chart();
}

void Widget::update_chart()
{
    auto dt=QDateTime::currentDateTime();
    auto now=static_cast<time_t>(dt.toTime_t());
    auto recs=BatteryRecord::GetInstance()->GetRecords(now-86400,now);

    //删除原来的线
    for(const auto&s:chart_->series()){
        chart_->removeSeries(s);
    }
    for(const auto &a:chart_->axes()){
        chart_->removeAxis(a);
    }

    QPen pen;
    {
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(3);
        pen.setColor(Qt::blue);
    }
    QPen pen_charge;
    {
        pen_charge.setStyle(Qt::SolidLine);
        pen_charge.setWidth(3);
        pen_charge.setColor(Qt::green);
    }
    //电量的折线
    std::vector<QLineSeries*> lines;
    int last_stat=-1;
    for(auto it=recs.cbegin();it!=recs.cend();++it){
        const auto& rec=*it;
        int st=std::get<2>(rec);//0用电池,1充电
        QLineSeries* line;
        if(st==last_stat){
            line=lines.back();
        }else{
            line = new QLineSeries();
            //连接上一个点防止看起来断掉
            if(it!=recs.cbegin()){
                auto r=*(it-1);
                line->append(std::get<0>(r)*1000,std::get<1>(r));
            }
            if(st==1){//充电
                line->setPen(pen_charge);
            }else{//用电
                line->setPen(pen);
            }
            lines.emplace_back(line);
        }
        line->append(std::get<0>(rec)*1000,std::get<1>(rec));
        last_stat=st;
    }

    for(auto &line:lines){
        chart_->addSeries(line);
    }

    dt.addSecs(3600*-12).toString("HH:mm");

    QDateTimeAxis *axisX = new  QDateTimeAxis;
    {
        axisX->setTickCount(9);
        axisX->setRange(dt.addDays(-1),dt);
        axisX->setFormat("HH:mm");
    }
    chart_->addAxis(axisX,Qt::AlignBottom);
    for(auto &line:lines){
        line->attachAxis(axisX);
    }

    QValueAxis *axisY = new QValueAxis;
    {
        axisY->setRange(0, 100);
        axisY->setMin(0);
        axisY->setMax(100);
        axisY->setLabelFormat("%d");
        axisY->setTickCount(11);//10格子
    }
    chart_->addAxis(axisY,Qt::AlignLeft);
    for(auto &line:lines){
        line->attachAxis(axisY);
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
    cf->color_charging=color;
    cf->Save("config.ini");
    IconTool::ClearCache();
}

void Widget::selected_bkc_b(const QColor &color)
{
    auto cf=Config::GetInstance();
    cf->color_us_bt=color;
    cf->Save("config.ini");
    IconTool::ClearCache();
}

void Widget::selected_fc_u(const QColor &color)
{
    auto cf=Config::GetInstance();
    cf->color_font_u=color;
    cf->Save("config.ini");
    IconTool::ClearCache();
}

void Widget::selected_fc_c(const QColor &color)
{
    auto cf=Config::GetInstance();
    cf->color_font_c=color;
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
//    qDebug()<<"show "<<QString::number(cf->color_charging.rgba(),16);
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
    update_chart();
}

//点击选择使用电池时的字体颜色
void Widget::on_btn_color_font_u_clicked()
{
    auto cf=Config::GetInstance();
    cld_f_u_->setCurrentColor(cf->color_font_u);
    cld_f_u_->show();
}

//点击选择充电时的字体颜色
void Widget::on_btn_color_font_c_clicked()
{
    auto cf =Config::GetInstance();
    cld_f_c_->setCurrentColor(cf->color_font_c);
    cld_f_c_->show();
}
