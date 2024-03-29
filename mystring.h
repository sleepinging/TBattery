//字符串操作

#if !defined(__H__MYSTRING__H__)
#define __H__MYSTRING__H__

#include <string>
#include <vector>
#include <sstream>

//将1个字符转为数字(16进制)
unsigned char atohex(char c);

//将1个字符转为数字(10进制)
unsigned char atodec(char c);

//将一个字节转为2个字符(16进制小写)
void hextoa(unsigned char h, char buf[2]);

//将一个字节转为2个字符(16进制大写)
void hextoA(unsigned char h, char buf[2]);

//分割字符串
void split(const std::string &s, const std::string &c, std::vector<std::string> &v);

//去除首尾空格
std::string &trim(std::string &s);

//URLEncode
std::string UrlEncode(const std::string &str);

//断言
void myasscrt(bool f, const char *msg = "asscrt wrong");

//校验和
uint16_t in_checksum(const void *buf, int len);

//字符串转换
template <typename S, typename D>
D mytrans(const S &s)
{
    D d;
    std::stringstream ss;
    ss << s;
    ss >> d;
    return d;
}

//字符串转换
template <typename S, typename D>
D mytrans(S &&s)
{
    D d;
    std::stringstream ss;
    ss << s;
    ss >> d;
    return d;
}

#endif // __H__MYSTRING__H__
