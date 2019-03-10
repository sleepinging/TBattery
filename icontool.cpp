#include "icontool.h"

#include <QPainter>
#include <QString>

#include "config.h"

std::unordered_map<int,QIcon> IconTool::ico_map;
QMutex IconTool::mtx_;

IconTool::IconTool()
{

}

QIcon IconTool::GenIcon(int num, bool charging)
{
//    return genIcon(num,charging);
    int key=(num<<16)+charging;
    QIcon ico;
    mtx_.lock();
    auto itm=ico_map.find(key);
    if(itm!=ico_map.end()){//在缓存里面
        ico= itm->second;
    }else{//不在缓存
        ico=genIcon(num,charging);
        ico_map[key]=ico;
    }
    mtx_.unlock();
    return ico;
}

void IconTool::ClearCache()
{
    mtx_.lock();
    ico_map.clear();
    mtx_.unlock();
}

QIcon IconTool::genIcon(int num, bool charging)
{
    //字体颜色
    QColor fc=Qt::black;

    QColor bk=charging?
        Config::GetInstance()->color_charging:
        Config::GetInstance()->color_us_bt;

    static QSize size(32,32); //指定图片大小;
    static QImage image(size,QImage::Format_ARGB32);
    //以ARGB32格式构造一个QImage
    image.fill(bk);//填充图片背景,120/250为透明度
    QPainter painter(&image); //为这个QImage构造一个QPainter
    //    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    //设置画刷的组合模式CompositionMode_SourceOut这个模式为目标图像在上。
    //改变组合模式和上面的填充方式可以画出透明的图片。

    //改变画笔和字体
    QPen pen = painter.pen();
    pen.setColor(fc);
    QFont font = painter.font();
    font.setBold(true);//加粗
    font.setPixelSize(32);//改变字体大小

    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(image.rect(),Qt::AlignCenter,QString::number(num));

    static QPixmap qp;
    qp=QPixmap::fromImage(image);
    static QIcon ic;
    ic=qp;
    return ic;
}
