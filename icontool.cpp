#include "icontool.h"

#include <QPainter>
#include <QString>

std::unordered_map<int,QIcon> IconTool::ico_map;

IconTool::IconTool()
{

}

QIcon IconTool::GenIcon(int num, bool charging)
{
    //字体颜色
    QColor fc=Qt::black;

    QColor bk=charging?qRgba(0xaf,0xff,0xaf,250):qRgba(0xff,0xff,0xff,250);

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
