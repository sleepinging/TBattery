#include "icontool.h"

#include <QPainter>
#include <QString>

IconTool::IconTool()
{

}

QIcon IconTool::GenIcon(int num, const QColor &bk)
{
    //字体颜色
    QColor fc=Qt::black;

    QSize size(50,50); //指定图片大小;
    QImage image(size,QImage::Format_ARGB32);
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
    font.setPixelSize(50);//改变字体大小

    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(image.rect(),Qt::AlignCenter,QString::number(num));

    QPixmap qp=QPixmap::fromImage(image);
    return qp;
}
