#include "config.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <QString>

#include "mystring.h"
#include "tfile.h"

Config* Config::is_=nullptr;

using namespace std;

Config::Config(){

}

Config::~Config(){

}

//从字符串读取配置
bool Config::LoadFromString(const std::string &str)
{
    stringstream ss(str);
    string line;
    while (getline(ss,line)){
        if(!handleline(line)){
            break;
        }
    }
    return true;
}

static string Color2Str(const QRgb& c){
    return QString::number(c,16).toStdString();
}

static QRgb Str2Color(const string& c){
    return QString(c.c_str()).toUInt(nullptr,16);
}

//处理一行,返回false停止
bool Config::handleline(const std::string &line){
    string key, value;
    vector<string> vec;
    split(line, "=", vec);
    if(vec.size()<2){
        return true;
    }
    //转小写
    key = vec[0];
    transform(vec[0].cbegin(), vec[0].cend(), key.begin(), ::tolower);
    value = vec[1];
    //transform(vec[1].cbegin(), vec[1].cend(), value.begin(), ::tolower);

    //去除空格
    trim(key);
    trim(value);

    //跳过注释
    if (
            key[0] == '#' ||
            key[0] == ';' ||
            key[0] == '\'' ||
            (key[0] == '/' && key[1] == '/')
            )
    {
        return true;
    }

    // cout << "key:" << key << ",value:" << value << endl;

    if (key == "back_color_charging")
    {
        color_charging=Str2Color(value);
        return true;
    }
    else if (key == "back_color_using")
    {
        color_us_bt=Str2Color(value);
        return true;
    }
    else if (key == "color_font_u")
    {
        color_font_u=Str2Color(value);
        return true;
    }

    else if (key == "end")
    {
        return false;
    }

    kvmap[key] = std::move(value);

    return true;
}

//检查正确性
bool Config::check()
{
    // if(deviceId==""){
    //     cout << "deviceid can't small than 0" << endl;
    //     return false;
    // }
    // if (ListenPort<=0){
    //     cout << "port can't small than 0" << endl;
    //     return false;
    // }

    return true;
}

//初始化
bool Config::Init(const std::string &cf,bool show)
{
    auto cfg = new Config();
    string str;

    //可以不存在，返回空
    str = TFile::ReadAllString(cf);

    //显示内容
    if (show)
    {
        cout << "config data:"<<endl << str << endl;
    }

    if (!cfg->LoadFromString(str))
    {
        delete cfg;
        cfg = nullptr;
        return false;
    }

    if (!cfg->check()){
        delete cfg;
        cfg = nullptr;
        return false;
    }

    is_ = cfg;
    return true;
}

Config* Config::GetInstance(){
    if(!is_){
        //is_=new Config();
        cout << "you must init config first!" << endl;
        throw "you must init config first!";
    }
    return is_;
}

int Config::Save(const string &cf)
{
    ofstream of(cf);

    of<<"back_color_using="<<Color2Str(is_->color_us_bt) <<"\n";
    of<<"back_color_charging="<<Color2Str(is_->color_charging) <<"\n";
    of<<"color_font_u="<<Color2Str(is_->color_font_u) <<"\n";

    return true;
}

int Config::CleanUp(){
    delete is_;
    is_=nullptr;

    return 0;
}
