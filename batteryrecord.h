#ifndef BATTERYRECORD_H
#define BATTERYRECORD_H

#include <time.h>

#include <vector>
#include <utility>

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
    //获取电池使用记录
    auto GetRecords(time_t start,time_t end)->std::vector<std::pair<time_t,unsigned char> >;
public:
    static int Init(const char* dbname="data.db");
    static BatteryRecord* GetInstance();
private:
    static BatteryRecord* is_;
};

#endif // BATTERYRECORD_H
