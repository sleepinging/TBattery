#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QSystemTrayIcon>
#include <QTimer>
#include <QMenu>
#include <QColorDialog>

class BatteryEvent;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    BatteryEvent *btevt_=nullptr;
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private slots:
    void on_btn_scbc_clicked();

    void on_btn_subc_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    void updatebtshow();
    void showbtinfo();

    //初始化托盘
    void inittray();

    //初始化电量记录模块
    void init_bt_rec();

    //显示主窗口
    void showmain();

    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent * event) override;

    void onactivetray(QSystemTrayIcon::ActivationReason reason);

    //选择完毕充电背景色
    void selected_bkc_c(const QColor &color);

    //选择完毕电池背景色
    void selected_bkc_b(const QColor &color);

    //保存电量记录
    static void save_record();

    void on_select_tab_rec(int index);

private:
    Ui::Widget *ui;
    QSystemTrayIcon *sti_=nullptr;
    //更新电量的定时器
    QTimer * timer_=nullptr;
    //保存记录的定时器
    QTimer* timer_save_record_=nullptr;
    //右键菜单
    QMenu* menu_=nullptr;
    //充电背景颜色选择
    QColorDialog* cld_c_=nullptr;
    //电池背景颜色选择
    QColorDialog* cld_b_=nullptr;
};

#endif // WIDGET_H
