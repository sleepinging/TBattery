//文件读写类

#if !defined(__H__TFILE__H__)
#define __H__TFILE__H__

#include <string>
#include <vector>
#include <fstream>
#include <functional>

//返回false表示不要继续读取了
using linehandler = std::function<bool(const std::string &)>;

class TFile
{
private:

public:
  TFile();
  ~TFile();

public:
  //读取所有字符串
  static std::string ReadAllString(const std::string &filename);

  //按行读取,每行调用函数，读取lines行,负数表示读取到最后一行,返回读取的行数
  static int ReadLines(const std::string &filename, const linehandler &lh,int lines=-1);

  //   //写入字符串

  //   //读取所有内容
  //   static std::vector<unsigned char> ReanAll(const std::string &filename);

  //   //写入字节
};

#endif // __H__TFILE__H__
