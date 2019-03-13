#ifndef BATTERYRECORD_H
#define BATTERYRECORD_H

#include <time.h>

#include <QtSql>

//电量记录 时间戳-电量
class BatteryRecord
{
private:
    QSqlDatabase db_;
    QSqlQuery* qry_=nullptr;
public:
    BatteryRecord(const char* dbname);
    ~BatteryRecord();
public:
    int AddRecord(time_t t,int percent);
public:
    static int Init(const char* dbname="data.db");
    static BatteryRecord* GetInstance();
private:
    static BatteryRecord* is_;
};

#endif // BATTERYRECORD_H
