#include "batteryrecord.h"

#include <QDebug>

BatteryRecord* BatteryRecord::is_=nullptr;

BatteryRecord::BatteryRecord(const char *dbname)
{
    db_=QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbname);
//    db_.setUserName("XingYeZhiXia");
    //    db_.setPassword("123456");
    //打开连接
    if(!db_.open())
    {
        qDebug() << db_.lastError();
        throw "open database failed";
    }
    qry_=new QSqlQuery(db_);
    //创建table
    qry_->prepare("CREATE TABLE IF NOT EXISTS record ( time DATETIME PRIMARY KEY UNIQUE NOT NULL, percent INTEGER NOT NULL);");
    if(!qry_->exec()){
        qDebug() << qry_->lastError();
        throw "create table failed";
    }
}

BatteryRecord::~BatteryRecord()
{
    db_.close();
    delete qry_;
    qry_=nullptr;
}

int BatteryRecord::AddRecord(time_t t, int percent)
{
    auto sql="INSERT INTO record (time, percent) VALUES ('"+QString::number(t)+"','"+QString::number(percent)+"')";
//    qDebug()<<sql;
    qry_->prepare(sql);
    if(!qry_->exec()){
        qDebug() << qry_->lastError();
        return -1;
    }
    return 0;
}

int BatteryRecord::Init(const char *dbname)
{
    if(is_==nullptr){
        is_=new BatteryRecord(dbname);
    }
    return 0;
}

BatteryRecord *BatteryRecord::GetInstance()
{
    if(is_==nullptr){
        qDebug()<<"you must init BatteryRecord first";
    }
    return is_;
}
