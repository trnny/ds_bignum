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

bignum::bignum(/* args */)
{
    negative = false;
    data = new int[1]{0};
    mlength = 0;
    vlength = 1;
    vi64 = 0;
}

/*  保留该构造函数主要是为了方便 
    能完成形如`bignum a = -123`的构造方法 
 */
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

/*  真正能构造大数的构造方法 
    但是奇怪的是形如 `bignum a("12345")1 构造通过 
                  `bignum b = "12345"` 却不能通过 
    猜测是由于const char* 会让编译器觉得需要用int64_t构造函数
 */
bignum::bignum(const string& num){
    if(!_isnumstr(num)){
        bignum();
        return;
    }
    negative = false;
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
        data[i] = atoi(num.substr(a,9).c_str());
        a += 9;
    }
}

bignum::~bignum()
{
    if(NULL != data){
        delete[] data;
        data = NULL;
    }
}

/*  完成到string的转换
    但是在`cout`时却没有达到预期的结果
    应该是`cout`时编译器调用了int64_t的隐式转换
 */
bignum::operator string() {
    stringstream ss;
    if(negative) ss << '-';
    for(int i=0;i<mlength;i++){
        ss << data[i] << ',';
    }
    return ss.str();
}

/*  完成到int64_t的转换
    由于这个函数存在,使得`cout`时想输出string得在前面加(string)
 */
bignum::operator int64_t() {
    if(!negative) return vi64;
    if(vi64 != 9223372036854775807) return -vi64;
    if(mlength == 3 && data[0] == 9 && data[1] == 223372036 && data[2] == 854775807) return -vi64;
    return - vi64;
}

/*
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
*/

bignum bignum::operator-(){
    bignum res(*this);
    res.negative = !negative;
    return res;
}

bool bignum::operator<(const bignum& num){
    if(negative && !num.negative) return true;
    if(!negative && num.negative) return false;
    if(vlength != num.vlength)    return !negative == vlength < num.vlength;
    for(int i=0;i<mlength;i++)  
        if(data[i] != num.data[i]) return !negative == data[i] < num.data[i];
    return false;
}
bool bignum::operator==(const bignum& num){
    if(negative != num.negative || vlength != num.vlength || vi64 != num.vi64) return false;
    for(int i = 0;i < mlength;i++)
        if(data[i] != num.data[i]) return false;
    return true;
}
bool bignum::operator>(const bignum& num){
    if(negative && !num.negative) return false;                 //  负数小于正数
    if(!negative && num.negative) return true;                  //  正数大于负数
    if(vlength != num.vlength)    return !negative == vlength > num.vlength;    //  正数更长的大
    for(int i = 0;i < mlength;i++)  
        if(data[i] != num.data[i]) return !negative == data[i] > num.data[i];   //  正数更大的大
    return false;                                               //  相等
}

bool bignum::operator<=(const bignum& num){
    if(negative && !num.negative) return true;
    if(!negative && num.negative) return false;
    if(vlength != num.vlength)    return !negative == vlength < num.vlength;
    for(int i=0;i<mlength;i++)  
        if(data[i] != num.data[i]) return !negative == data[i] < num.data[i];
    return true;
}
bool bignum::operator!=(const bignum& num){
    return !(*this == num);
}
bool bignum::operator>=(const bignum& num){
    if(negative && !num.negative) return false;
    if(!negative && num.negative) return true;
    if(vlength != num.vlength)    return !negative == vlength > num.vlength;
    for(int i = 0;i < mlength;i++)  
        if(data[i] != num.data[i]) return !negative == data[i] > num.data[i];
    return true; 
}

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

// int main(){
//     bignum a("123456789012345678922334499988866677744409875454012");
//     bignum b = -122223747485;
//     string s = a;
//     int64_t i = b;
//     cout << s << endl;
//     cout << (string)a << '\n' << i << '\n' << b << endl;
//     return 0;
// }