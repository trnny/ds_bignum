#include "bignum.h"

int main(){
    int64_t a1 = -80538738812075974,
        a2 = 80435758145817515,
        a3 = 12602123297335631;
    bignum b1 = -80538738812075974,
        b2 = 80435758145817515,
        b3 = 12602123297335631;
    bignum c1("-80538738812075974"),
        c2("80435758145817515"),
        c3("12602123297335631");
    cout << "show a1, a2, a3 & b1, b2, b3:" << endl;
    cout << a1 << "  " << a2 << "  " << a3 << endl;
    cout << b1 << "  " << b2 << "  " << b3 << endl;
    cout << (string)c1 << "  " << (string)c2 << "  " << (string)c3 << endl;
    cout << " ****************************************************** " << endl;
    cout << a1*a1*a1 + a2*a2*a2 + a3*a3*a3 << endl;
    cout << (string)(b1*b1*b1 + b2*b2*b2 + b3*b3*b3) << endl;
    return 0;
}

//  编译指令    放弃了 不知道在linux下多文件怎么自动汇编 
//  百度了要用到make,麻烦死了,主要是不想目录下那么多杂七杂八的文件
//  g++ -g /home/trnny/Documents/VSCode/C++/ds_bignum/main.cpp /home/trnny/Documents/VSCode/C++/ds_bignum/bignum.cpp -std=c++11 -o /home/trnny/Documents/VSCode/C++/ds_bignum/main