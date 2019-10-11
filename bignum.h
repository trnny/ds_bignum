#pragma once
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class bignum
{
private:
    bool negative;              // 正负
    int *data;                  // 保存数各个位
    int mlength;                // 数组长度 
    int vlength;                // 数长度 10进制
    int64_t vi64;               // c++长整数数值(正数部分,为方便计算)
public:
    const static int S;         // 进制  1000000000 十亿
    bignum(const string& num);  // 推荐的构造函数
    bignum();
    bignum(const bignum& num);
    bignum(int64_t num);        // 简单起见 超出int64范围就按照边缘给出
    ~bignum();
    /*explicit*/ operator int64_t();        // cout 时将默认以int64输出 虽然有转向string的定义 但隐式还是会只执行这个
    operator string();
    // 算术运算符
    bignum operator+(const bignum& num);
    bignum operator-(const bignum& num);
    bignum operator*(const bignum& num);
    // 负运算符
    bignum operator-();
    // 比较运算符
    bool operator<(const bignum& num);
    bool operator==(const bignum& num);
    bool operator>(const bignum& num);
    bool operator<=(const bignum& num);
    bool operator!=(const bignum& num);
    bool operator>=(const bignum& num);
};