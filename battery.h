#if !defined(__H__BATTERY__H__)
#define __H__BATTERY__H__

#include <QMutex>

class Battery
{
public:
    enum class Status:unsigned char
    {
        CHARGING=1,
        UNCHARGING=0,
        UNKNOW=255
    };
public:
    static Battery::Status status;
    static unsigned char percent;
private:
    /* data */
public:
    Battery(/* args */);
    ~Battery();
public:
    //成功返回非0
    static int Update();
private:
    static QMutex *mtx_;
};

#endif // __H__BATTERY__H__
