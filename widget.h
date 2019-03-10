#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QSystemTrayIcon>
#include <QTimer>
#include <QMenu>

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

private:
    void updatebtshow();
    void showbtinfo();

    //初始化托盘
    void inittray();

    //显示主窗口
    void showmain();

    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent * event) override;

    void onactivetray(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::Widget *ui;
    QSystemTrayIcon *sti_=nullptr;
    QTimer * timer_=nullptr;
    //右键菜单
    QMenu* menu_=nullptr;
};

#endif // WIDGET_H
