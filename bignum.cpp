#include "bignum.h"

const int bignum::S  = 1000000000;

static int _getvi64len(int64_t num){
    int l = 0;
    do{
        num /= 10;
        l++;
    }while (num != 0);
    return l;
}

static bool _isnumstr(const string& num){       // 判断输入的字符串和不合格 (简单起见,只允许输入纯数字或者负号加纯数字)
    if(num[0] != '-' && (num[0] < 0x30 || num[0] > 0x39)) return false;
    for(int i =1; i < num.length(); i++)
    {  
        if(num[i]<0x30||num[i]>0x39) return false;
    }
    return true;
}

/* 
static int _getvi64mlen(int64_t num){   // 该函数可以被上面函数取代
    int l = 0;
    do{
        num /= bignum::S;
        l++;
    }while (num != 0);
    return l;
}
*/

bignum::bignum(/* args */)
{
    negative = false;
    data = NULL;
    mlength = 0;
    vlength = 1;
    vi64 = 0;
}

bignum::bignum(int64_t num){
    if(num == 0) {
        bignum();
        return;
    }
    if(num < 0) {
        negative = true;
        num = -num;
    }else {
        negative = false;
    }
    vi64 = num;
    vlength = _getvi64len(vi64);
    // mlength = _getvi64mlen(vi64);
    mlength = vlength / 9 + 1;
    data = new int[mlength];
    for(int i = 0;i<mlength;i++){
        data[mlength - i - 1] = num % S;
        num /= S;
    }
}

bignum::bignum(const string& num){
    if(_isnumstr(num)){
        bignum();
        return;
    }
    int a = 0;
    if(num[0] == '-'){
        negative = true;
        a = 1;
    }
    vlength = num.length() - a;
    mlength = vlength / 9 + 1;
    stringstream ss;
    ss << num.substr(a,vlength).c_str();
    ss >> vi64;
    data = new int[mlength];
    data[0] = atoi(num.substr(a,vlength % 9 - a).c_str());
    a = vlength % 9;
    for(int i = 1;i < mlength;i++){
        a += 9;
        data[i] = atoi(num.substr(a,9).c_str());
    }
}

bignum::~bignum()
{
    if(NULL != data){
        delete[] data;
        data = NULL;
    }
}

bignum::operator string() {
    stringstream ss;
    if(negative) ss << '-';
    for(int i=0;i<mlength;i++){
        ss << data[i];
    }
    return ss.str();
}

bignum::operator int64_t() {
    if(!negative) return vi64;
    if(mlength == 3 && data[0] == 9 && data[1] == 223372036 && data[2] == 854775807) return -vi64;
    return - vi64 - 1;
}

bignum bignum::operator+(const bignum& num){    // 简单起见 只处理"正数与正数"相加
    if(num.negative == negative){
        // 符号相同 简单相加
        bignum res;
        res.negative = negative;
    }else if(negative){
        // 自己负数 别人正数
    }else{
        // 自己正数 别人负数
    }
}

bignum bignum::operator-(const bignum& num){    // 简单起见 只处理"正数与正数"相减
    if(num.negative == negative){
        bignum res;
        res.negative = negative;
    }else if(negative){
        // 自己负数 别人正数 
    }else {
        // 自己正数 别人负数
    }
}

bignum bignum::operator*(const bignum& num){
    
}

bignum bignum::operator-(){
    bignum res(*this);
    res.negative = !negative;
    return res;
}

bool bignum::operator<(const bignum& num){
    
}
bool bignum::operator==(const bignum& num){}
bool bignum::operator>(const bignum& num){}

bignum::bignum(const bignum& num){
    negative = num.negative;
    mlength = num.mlength;
    vlength = num.vlength;
    vi64 = num.vi64;
    data = new int[mlength];
    for(int i=0;i<mlength;i++){
        data[i] = num.data[i];
    }
}