#ifndef BATTERYEVENT_H
#define BATTERYEVENT_H

#include <QObject>
#include <QAbstractNativeEventFilter>

class BatteryEvent :public QObject,public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    BatteryEvent();
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;
signals:
    void PowerChanged();
};

#endif // BATTERYEVENT_H
