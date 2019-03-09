#ifndef ICONTOOL_H
#define ICONTOOL_H

#include <unordered_map>

#include <QIcon>

class IconTool
{
public:
    IconTool();
    static QIcon GenIcon(int num,bool charging=false);
private:
    //缓存的图片
    static std::unordered_map<int,QIcon> ico_map;
};

#endif // ICONTOOL_H
