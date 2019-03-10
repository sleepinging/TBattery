#include "tfile.h"

#include <fstream>
#include <sstream>

using namespace std;

TFile::TFile()
{
}

TFile::~TFile()
{
}

//读取所有字符串
std::string TFile::ReadAllString(const std::string &filename)
{
    stringstream ss;
    fstream fs(filename, ios::in);
    if(!fs.is_open()){
        return "";
    }
    ss<<fs.rdbuf();
    return ss.str();
}

//按行读取,每行调用函数，读取lines行,-1表示读取到最后一行,返回读取的行数
int TFile::ReadLines(const std::string &filename, const linehandler &lh, int lines)
{
    std::ifstream fin(filename);
    if (!fin)
    {
        fin.close(); //也有别的办法可以让指针指到文件开头
        fin.open(filename, std::ios::in);
        if(!fin.is_open()){
            return 0;
        }
    }
    int c = 0;
    std::string line;
    while (getline(fin, line)&&lines--!=0)
    {
        ++c;
        if (!lh(line))
        {
            break;
        }
    }
    fin.close();
    return c;
}
