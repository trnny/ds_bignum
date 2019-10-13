#include "bignum.h"

int main(){
    string a = (bignum)"199804141998041419980414199804141998041419980414" * (bignum)"1627405150" * (bignum)"15895567150";
    string b = (bignum)"99999999999999999999999999999999999999999999999999999" * (bignum)"1234567890987654321";
    string c = (bignum)"10000000000000000000000000000" + (bignum)222222 + (bignum)33333000000000000 + (bignum)"990000000000000000000000";
    string d = (bignum)"500000000000000" * (bignum)300004000 - (bignum)"100000000000000000000000000000000000000000000000000000000000";
    cout << a << '\n' << b << '\n' << c << '\n' << d << endl;
    return 0;
}

//  编译指令    放弃了 不知道在linux下多文件怎么自动汇编 
//  百度了要用到make,麻烦死了,主要是不想目录下那么多杂七杂八的文件
//  g++ -g /home/trnny/Documents/VSCode/C++/ds_bignum/main.cpp /home/trnny/Documents/VSCode/C++/ds_bignum/bignum.cpp -std=c++11 -o /home/trnny/Documents/VSCode/C++/ds_bignum/main