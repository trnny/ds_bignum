#include "bignum.h"

int main(){
    bignum a("123456789012345678922334499988866677744409875454012");
    bignum b = 12345678901230000;
    bignum c("-35464575");
    bignum d = -122223747485;
    cout << (string)a << '\n' << b << '\n' << c << '\n' << d << endl;
    return 0;
}