/*
 * @Description: 配置文件读取
 * @Author: taowentao
 * @Date: 2019-01-14 13:45:22
 * @LastEditors: taowentao
 * @LastEditTime: 2019-01-17 17:39:03
 */

#if !defined(__H__CONFIG__H__)
#define __H__CONFIG__H__

#include <string>
#include <unordered_map>
#include <vector>

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
    //初始化,可以是文件或者http地址
    static bool Init(const std::string &cf = "config.ini", bool show=false);

    static Config *GetInstance();

  public:

    //其它配置项,key都会变成小写
    std::unordered_map<std::string, std::string> kvmap;

    //显示调试消息等级
    int infolevel = 0;

  private:
    static Config *is_;

  public:
    static int CleanUp();
};

#endif // __H__CONFIG__H__
