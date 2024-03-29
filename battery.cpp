#include "battery.h"

#include <windows.h>

using namespace std;

Battery::Status Battery::status;
unsigned char Battery::percent;
QMutex* Battery::mtx_=new QMutex();

Battery::Battery(/* args */)
{
}

Battery::~Battery()
{
}

int Battery::Update()
{
    int r = 0;
    static _SYSTEM_POWER_STATUS sps;
    mtx_->lock();
    r = GetSystemPowerStatus(&sps);
    status = static_cast<Battery::Status>(sps.ACLineStatus);
    percent = sps.BatteryLifePercent;
    mtx_->unlock();
    return r;
}
