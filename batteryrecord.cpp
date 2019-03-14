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

std::vector<std::pair<time_t, unsigned char> > BatteryRecord::GetRecords(time_t start, time_t end)
{
    std::vector<std::pair<time_t, unsigned char>> recs;
    qry_->prepare("SELECT * FROM record where time >= "+QString::number(start)
                  +" and time <= "+QString::number(end)+"");
    if(!qry_->exec())
    {
        qDebug() << qry_->lastError();
        return recs;
    }
    else
    {
        QSqlRecord rec = qry_->record();
//        int cols = rec.count();//字段数
//        for(int c=0; c<cols; c++)
//        {
//            qDebug() << QString("Column %1: %2").arg(c).arg(rec.fieldName(c));
//        }

        while (qry_->next()) {
            time_t t=qry_->value("time").toLongLong();
            unsigned char per=static_cast<unsigned char>(qry_->value("percent").toInt());
            recs.emplace_back(std::make_pair(t,per));
        }
    }

    return recs;

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
