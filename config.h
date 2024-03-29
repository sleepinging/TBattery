/*
 * @Description: 配置文件读取
 * @Author: taowentao
 * @Date: 2019-01-14 13:45:22
 * @LastEditors: taowentao
 * @LastEditTime: 2019-01-17 17:39:03
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>
#include <vector>

#include <QColor>
#include <QRgb>

//调试消息等级
namespace INFOLEVEL
{
}

class Config
{
  public:
    Config();
    ~Config();

    //从字符串读取配置
    bool LoadFromString(const std::string &str);

  private:
    //检查正确性
    bool check();

    //处理一行,返回false停止
    bool handleline(const std::string &line);

  public:
    //初始化
    static bool Init(const std::string &cf = "config.ini", bool show=false);

    static Config *GetInstance();

    //保存
    static int Save(const std::string &cf = "config.ini");

  public:

    //充电颜色
    QColor color_charging=QColor(0xaf,0xff,0xaf,250);

    //使用电池的颜色
    QColor color_us_bt=QColor(0xff,0xff,0xff,250);

    //用电池时字体颜色
    QColor color_font_u=QColor(0x00,0x00,0x00,0xff);

    //用充电时字体颜色
    QColor color_font_c=QColor(0x00,0x00,0x00,0xff);

    //背景不透明度
//    unsigned char background_transparency=0;

    //其它配置项,key都会变成小写
    std::unordered_map<std::string, std::string> kvmap;

    //显示调试消息等级
    int infolevel = 0;

  private:
    static Config *is_;

  public:
    static int CleanUp();
};

#endif // CONFIG_H
