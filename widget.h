#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QSystemTrayIcon>
#include <QTimer>

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
    ~Widget();

private:
    void updatebtshow();
    void showbtinfo();

    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent * event) override;

    void onactivetray(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::Widget *ui;
    QSystemTrayIcon *sti_=nullptr;
    QTimer * timer_=nullptr;
};

#endif // WIDGET_H
