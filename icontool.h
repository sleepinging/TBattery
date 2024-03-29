#ifndef ICONTOOL_H
#define ICONTOOL_H

#include <unordered_map>

#include <QIcon>
#include <QMutex>

class IconTool
{
public:
    IconTool();
    //从缓存获取图片
    static QIcon GenIcon(int num,bool charging=false);
    //清除缓存
    static void ClearCache();
private:
    //生成图片
    static QIcon genIcon(int num,bool charging);
private:
    //缓存的图片
    static std::unordered_map<int,QIcon> ico_map;
    //锁
    static QMutex mtx_;
};

#endif // ICONTOOL_H
