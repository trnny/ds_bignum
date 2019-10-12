#include "bignum.h"

int main(){
    bignum a("123456789012345678922334499988866677744409875454012");
    bignum b = 12345678901230000;
    bignum c("-35464575");
    bignum d = -122223747485;
    cout << (string)a << '\n' << b << '\n' << c << '\n' << d << endl;
    return 0;
}

//  编译指令    放弃了 不知道在linux下多文件怎么自动汇编 
//  百度了要用到make,麻烦死了,主要是不想目录下那么多杂七杂八的文件
//  g++ -g /home/trnny/Documents/VSCode/C++/ds_bignum/main.cpp /home/trnny/Documents/VSCode/C++/ds_bignum/bignum.cpp -std=c++11 -o /home/trnny/Documents/VSCode/C++/ds_bignum/main