#include "mystring.h"

// #include <iostream>

using namespace std;

//将1个字符转为数字(16进制)
unsigned char atohex(char c)
{
    unsigned char s = 0;
    if (c >= '0' && c <= '9')
    {
        s = c - '0';
    }
    else if (c >= 'a' && c <= 'z')
    {
        s = c - 'a' + 10;
    }
    else if (c >= 'A' && c <= 'Z')
    {
        s = c - 'A' + 10;
    }
    return s;
}

//将1个字符转为数字(10进制)
unsigned char atodec(char c)
{
    return c - '0';
}

//将一个字节转为2个字符(16进制小写)
void hextoa(unsigned char h, char buf[2])
{
    unsigned char a = h >> 4;
    unsigned char b = h & 0x0f;

    if (a < 10)
    {
        buf[0] = '0' + a;
    }
    else
    {
        buf[0] = 'a' + a - 10;
    }

    if (b < 10)
    {
        buf[1] = '0' + b;
    }
    else
    {
        buf[1] = 'a' + b - 10;
    }

    return;
}

//将一个字节转为2个字符(16进制大写)
void hextoA(unsigned char h, char buf[2])
{
    unsigned char a = h >> 4;
    unsigned char b = h & 0x0f;

    if (a < 10)
    {
        buf[0] = '0' + a;
    }
    else
    {
        buf[0] = 'A' + a - 10;
    }

    if (b < 10)
    {
        buf[1] = '0' + b;
    }
    else
    {
        buf[1] = 'A' + b - 10;
    }

    return;
}

void split(const string &s, const string &c, vector<std::string> &v)
{
    string::size_type pos1, pos2;
    pos1 = 0;
    pos2 = s.find(c);
    while (string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

std::string &trim(std::string &s)
{
    if (s.empty())
        return s;
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

void myasscrt(bool f,const char* msg){
    if(!f){
        throw msg;
    }
}

uint16_t in_checksum(const void *buf, int len)
{
    myasscrt(len % 2 == 0,"len must 2 times");
    const uint16_t *data = static_cast<const uint16_t *>(buf);
    int sum = 0;
    for (int i = 0; i < len; i += 2)
    {
        sum += *data++;
    }
    // while (sum >> 16)
    sum = (sum & 0xFFFF) + (sum >> 16);
    myasscrt(sum <= 0xFFFF, "");
    return ~sum;
}

inline static unsigned char ToHex(unsigned char x)
{
    return x > 9 ? x + 55 : x + 48;
}

std::string UrlEncode(const std::string &str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}
