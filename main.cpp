#include "bignum.h"

int main(){
    string a = (bignum)"12345678901234567892233449998886667774440900000000" - (bignum)22334455;
    string b = (bignum)-12345678901230000 + (bignum)-472839548;
    string c = (bignum)"";
    string d = (bignum)"1000000000000000000000000000000000000000000000000000000000000000000000000000" - (bignum)"9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999";
    string e = (bignum)"999999999" - (bignum)1;
    cout << a << '\n' << b << '\n' << c << '\n' << d << '\n' << e << endl;
    return 0;
}

//  编译指令    放弃了 不知道在linux下多文件怎么自动汇编 
//  百度了要用到make,麻烦死了,主要是不想目录下那么多杂七杂八的文件
//  g++ -g /home/trnny/Documents/VSCode/C++/ds_bignum/main.cpp /home/trnny/Documents/VSCode/C++/ds_bignum/bignum.cpp -std=c++11 -o /home/trnny/Documents/VSCode/C++/ds_bignum/main