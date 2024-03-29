#include "icontool.h"

#include <QPainter>
#include <QString>
#include <QDebug>

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

//void print_color(const QColor& c){
//    qDebug()<<c.red()<<","<<c.green()<<","<<c.blue()<<","<<c.alpha();
//}

QIcon IconTool::genIcon(int num, bool charging)
{
    auto cfg=Config::GetInstance();
    //字体颜色
    QColor fc=charging?cfg->color_font_c:cfg->color_font_u;
    //背景色
    QColor bk=charging?cfg->color_charging:cfg->color_us_bt;

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
    if(num>=100){
        font.setPixelSize(16);//3个数字的大小
    }else if(num>=10){
        font.setPixelSize(20);
    }else{
        font.setPixelSize(32);
    }
    pen.setWidth(2);

    painter.setPen(pen);
    painter.setFont(font);
    painter.setRenderHint(QPainter::Antialiasing);//设置圆滑绘制风格（抗锯齿）
    //画圆弧
    painter.drawArc(1,1,30,30,90*16,((num+2)/3*3-2)*3.6*16);//左上角在0,0,a=30,b=30,角度=电量*360°
    //画文字
    painter.drawText(image.rect(),Qt::AlignCenter,QString::number(num));

    static QPixmap qp;
    qp=QPixmap::fromImage(image);
    static QIcon ic;
    ic=qp;
    return ic;
}
