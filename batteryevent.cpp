#include "batteryevent.h"

#include <QString>
#include <QtDebug>

#include <windows.h>

BatteryEvent::BatteryEvent()
{

}

bool BatteryEvent::nativeEventFilter(const QByteArray &eventType, void *message, long *)
{
    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
    {
        MSG * pMsg = reinterpret_cast<MSG *>(message);
        if (pMsg->message == WM_POWERBROADCAST)
        {
//            qDebug()<<"power change"<<endl;
            emit PowerChanged();
        }
    }
    return false;
}
