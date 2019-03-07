#ifndef ICONTOOL_H
#define ICONTOOL_H

#include <QIcon>

class IconTool
{
public:
    IconTool();
    static QIcon GenIcon(int num,const QColor &bk);
};

#endif // ICONTOOL_H
