#include "battery.h"

#include <windows.h>

using namespace std;

Battery::Status Battery::status;
unsigned char Battery::percent;

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
    r = GetSystemPowerStatus(&sps);
    status = static_cast<Battery::Status>(sps.ACLineStatus);
    percent = sps.BatteryLifePercent;
    return r;
}
