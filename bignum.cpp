#include "bignum.h"

const int bignum::S  = 1000000000;

/*  判断一个数的十进制长度
 */
static int _getvi64len(int64_t num){
    int l = 0;
    do{
        num /= 10;
        l++;
    }while (num != 0);
    return l;
}

/*  判断输入的字符串符不符合要求
    简单起见,只允许输入纯数字或者负号加纯数字
 */
static bool _isnumstr(const string& num){ 
    if(num.length() == 0 || (num.length() == 1 && (num[0] == '-'|| num[0] == 0x30))) return false;     // 空的或者只有一个负号或0
    if(num[0] != '-' && (num[0] < 0x30 || num[0] > 0x39)) return false;         // 第一个不是负号也不是数字
    for(int i =1; i < num.length(); i++)
    {  
        if(num[i]<0x30||num[i]>0x39) return false;                              // 第一关之后不是数字
    }
    return true;
}

bignum::bignum(/* args */)
{
    negative = false;
    data = new int[1]{0};      // 编译器偶尔自动补0 ???
    mlength = 1;
    vlength = 1;
    vi64 = 0;
}

/*  保留该构造函数主要是为了方便 
    能完成形如`bignum a = -123`的构造方法 
 */
bignum::bignum(int64_t num){            
    if(num == 0) {
        new (this)bignum();     // 使用默认的构造函数
        return;
    }
    if(num < 0) {
        negative = true;
        num = -num;
        if(num < 0){            // 针对 num = -9223372036854775808 的特殊情况
            mlength = 3;
            vlength = 19;
            vi64 = 9223372036854775807;
            data = new int[3]{9, 223372036, 854775808};
            return;
        }
    }else {
        negative = false;
    }
    vi64 = num;
    vlength = _getvi64len(vi64);
    // mlength = _getvi64mlen(vi64);
    mlength = (vlength - 1) / 9 + 1;
    data = new int[mlength];
    for(int i = 0;i<mlength;i++){
        data[mlength - i - 1] = num % S;
        num /= S;
    }
}

/*  真正能构造大数的构造方法 
    但是奇怪的是形如 `bignum a("12345") 构造通过 
                  `bignum b = "12345"` 却不能通过 
    猜测是由于const char* 会让编译器觉得需要用int64_t构造函数
 */
bignum::bignum(const string& num){
    if(!_isnumstr(num)){
        new (this)bignum();     // 输入不合格,使用默认的构造函数
        return;
    }
    negative = false;
    int a = 0;                  // 记录跳过的长度
    if(num[0] == '-'){
        negative = true;
        a = 1;
    }
    while(num[a] == 0x30 && ++a < num.length() - 1);            // 消除前头无意义的"0" , a最大为num.length() - 1;
    vlength = num.length() - a;             // 一定会大于等于1(该数字一定会有长度)
    mlength = (vlength - 1) / 9 + 1;        // 不足9为1, 等于9为1, 超过9`加1`
    stringstream ss;
    ss << num.substr(a,vlength).c_str();
    ss >> vi64;
    data = new int[mlength];
    data[0] = atoi(num.substr(a, (vlength - 1) % 9 + 1).c_str());       // 把头切下来, 不足或等于9为本身,超过9`减`9
    a += (vlength - 1) % 9 + 1;          // 切头后的开始索引, 后面的与`9`对齐了
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
bignum::operator string() const {
    // if(NULL == data) return "0";         // 在默认的构造中同样给data分配了空间,不用检查是否为空了
    stringstream ss;
    if(negative) ss << '-';
    ss << data[0];                          // 第一个前面的0可以省
    for(int i = 1;i < mlength;i++){         // 这里得小心, 一开始马虎了, 长度不到9的int在转成字符串时会短(前面的0省略了)
        ss.width(9);
        ss.fill('0');
        ss << data[i];
    }
    return ss.str();
}

/*  完成到int64_t的转换
    由于这个函数存在,使得`cout`时想输出string得在前面加(string)
 */
bignum::operator int64_t() const {          // 除了 num = -9223372036854775808 时的特殊情况,正常返回
    if(!negative) return vi64;
    if(vi64 == 9223372036854775807 && mlength == 3 && data[0] == 9 && data[1] == 223372036 && data[2] == 854775808) return -vi64 - 1;
    return -vi64;
}

bignum& bignum::operator=(const bignum& num){
    if(this == &num) return *this;      // 指针相同
    if(*this == num) return *this;      // 值相等
    if(NULL != data) delete[] data;
    mlength = num.mlength;
    negative = num.negative;
    vlength = num.vlength;
    vi64 = num.vi64;
    data = new int[mlength];
    for(int i = 0; i < mlength; i++)
        data[i] = num.data[i];
    return *this;
}

bignum bignum::operator+(const bignum& num) const{          // 简单起见 只处理"正数与正数"相加
    if(num.negative == negative){
        bignum res;
        res.negative = negative;
        res.vi64 = vi64 + num.vi64;
        if(res.vi64 < 0) res.vi64 = 9223372036854775807;
        int carry = 0, _tvl_max/*可能的最大的10进制长度*/, _tml_max/*可能的最大10亿进制长度(即数组需要申请的长度)*/, _tml_big/*两个数中长度大的数组长度*/, _tml_min/*最小数组长度*/, _tvc/*每个10亿位上的计算结果值*/;
        int *_tdata, *_tdata_big, *_tdata_min;      //  分别是 临时申请的数组, 长度大的数组, 长度短的数组
        if(num.vlength>vlength){
            _tvl_max = num.vlength + 1;
            _tml_big = num.mlength;
            _tml_min = mlength;
            _tdata_big = num.data;
            _tdata_min = data;
        }else{
            _tvl_max = vlength + 1;
            _tml_big = mlength;
            _tml_min = num.mlength;
            _tdata_big = data;
            _tdata_min = num.data;
        }
        _tml_max = (_tvl_max - 1) / 9 + 1;      // 不超过9为1 超过9`加1`
        _tdata = new int[_tml_max]{0};
        for(int i=0;i<_tml_big;i++){
            _tvc = _tdata_big[_tml_big - i - 1] + carry;
            carry = 0;
            if(i<_tml_min) _tvc += _tdata_min[_tml_min - i - 1];
            if(_tvc >= S){
                carry = 1;
                _tvc -= S;
            }
            _tdata[_tml_max - i - 1] = _tvc;        // 从后往前填
        }
        if(carry) _tdata[0] = 1;            // 运算循环结束后还是有进位表示运算结果是进位的
        int _toffset = _tdata[0] == 0;      // _tdata[0] == 0 表示实际运算结果没有进位 申请的额外一空间是空
        res.mlength = _tml_max - _toffset;
        res.data = new int[res.mlength];
        for(int i=0;i<res.mlength;i++){
            res.data[i] = _tdata[i+_toffset];
        }
        res.vlength = _getvi64len(res.data[0]) + (res.mlength - 1) * 9;
        delete[] _tdata;
        return res;
    }
    else                            // 自己正数 别人负数 *this + num -> *this - (-num);
        return *this - -num;
}


bignum bignum::operator-(const bignum& num) const{          // 简单起见 只处理"正数与正数"相减 且"大数减小数"
    if(num.negative == negative){
        bignum res;
        int carry = 0, _tml_big, _tml_sml, _tvc;
        int *_tdata, *_tdata_big, *_tdata_sml;
        if(*this > num){
            res.negative = negative;
            _tml_big = mlength;
            _tml_sml = num.mlength;
            _tdata_big = data;
            _tdata_sml = num.data;
        }else{
            res.negative = !negative;
            _tml_big = num.mlength;
            _tml_sml = mlength;
            _tdata_big = num.data;
            _tdata_sml = data;
        }
        _tdata = new int[_tml_big];
        for(int i=0;i<_tml_big;i++){
            _tvc = _tdata_big[_tml_big - i -1] - carry;
            carry = 0;
            if(i<_tml_sml) _tvc -= _tdata_sml[_tml_sml - i - 1];
            if(_tvc < 0){
                carry = 1;
                _tvc += S;
            }
            _tdata[_tml_big - i - 1] = _tvc;
        }
        bool _tvs = false;
        int _offset;
        for(int i = 0;i<_tml_big;i++){
            if(!_tvs && _tdata[i]){
                _tvs = true;
                _offset = i;
                res.mlength = _tml_big - i;
                res.data = new int[res.mlength];
                res.data[0] = _tdata[i];
                res.vi64 = _tdata[i];
                res.vlength = _getvi64len(_tdata[i]);       // 这里不和加 乘采一样的操作是因为这步res.data不一定初始化,免得另作判断
                continue;
            }
            res.data[i - _offset] = _tdata[i];
            res.vlength += 9;
        }
        if(!_tvs)
            res.negative = false;       // 其余变量都在默认构造函数里初始化了
        else if(res.vlength>19)
            res.vi64 = 9223372036854775807;
        else{
            for(int i=1;i<res.mlength;i++){
                res.vi64 *= S;
                if(res.vi64 <= 0) break;
                res.vi64 += res.data[i];
            }
            if(res.vi64 <= 0)
                res.vi64 = 9223372036854775807;
        }
        delete[] _tdata;
        return res;
    }
    else                            // *this - num -> *this + -num;
        return *this + -num;
}

bignum bignum::operator*(const bignum& num) const{
    if(vi64 == 0 || num.vi64 == 0) return 0;        // 任意为0, 直接返回0
    bignum res;
    res.negative = negative!=num.negative;
    /* 定义数据 */
    int carry = 0, 
        _tvl_max = vlength + num.vlength,   // 长度上不会超出2数长度之和 不会小于2数长度之和-1
        _tvl_min = _tvl_max - 1, 
        _tvl_big, 
        _tvl_sml, 
        _tml_big, 
        _tml_max = (_tvl_max - 1) / 9 + 1;
    int *_tdata = new int[_tml_max]{0},        // 编译器已经帮补零了
        *_tdata_big;
    string _tstrnum_sml;
    int _tstrnum_len;
    if(vlength > num.vlength){
        _tvl_big = vlength;
        _tvl_sml = num.vlength;
        _tml_big = mlength;
        _tdata_big = data;
        _tstrnum_sml = (string)num;
    }else{
        _tvl_big = num.vlength;
        _tvl_sml = vlength;
        _tml_big = num.mlength;
        _tdata_big = num.data;
        _tstrnum_sml = (string)*this;
    }
    _tstrnum_len = _tstrnum_sml.length();
    int _pow10[9] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};   // 用于计算`位移`
    int _offset_outer, 
        _offset_inner;
    int64_t _tvi64c,        // 用来保存每位上与数组的乘
            _tvi64v;        // 用来存运算后的值
    int _tvi10;             // 乘数的数字 10以内
    for(int i=0;i<_tvl_sml;i++){            // 遍历乘数每个10进制数字
        // work
        _offset_outer = i / 9;
        _offset_inner = i % 9;              // 0 ~ 8
        _tvi10 = _tstrnum_sml[_tstrnum_len - i - 1] - 0x30;     // 第i个进行运算的数字 (即除数倒数i个)
        if(!_tvi10) continue;
        for(int j = 0;j<_tml_big;j++) {     // 遍历被乘数每个10亿进制`int`      乘积累加
            // work
            _tvi64c = (int64_t)_tdata_big[_tml_big - j -1] * _tvi10 + carry;        // 这里得先转换一下位数, 不然就是int相乘了,会溢出
            carry = 0;
            _tvi64c *= _pow10[_offset_inner];
            if(_tvi64c >= S){
                carry = _tvi64c / S;                            // 乘部分的进位
                _tvi64c %= S;
            }
            _tvi64v = _tdata[_tml_max - _offset_outer - j - 1] + _tvi64c;
            if(_tvi64v >= S){      // 累加部分的进位 
                carry++;                                        // 这里不合并到下面的for语句因为这里carry不确定
                _tvi64v -= S;
            }
            _tdata[_tml_max - _offset_outer - j - 1] = _tvi64v;
            for(int k = _tml_max - _offset_outer - j - 2; carry != 0;k--){      // 循环进位加到前头元素, 原则上不会越界,不需要判断k>=0
                _tdata[k] += carry;
                carry = 0;
                if(_tdata[k] >= S){
                    carry = 1;
                    _tdata[k] -= S;
                }
            }
        }
    }
    int _toffset = _tdata[0] == 0;              // 是否需要移位(_tdata申请空间比运算实际结果空间长1)
    res.mlength = _tml_max - _toffset;
    res.data = new int[res.mlength];
    for(int i=0;i<res.mlength;i++){
        res.data[i] = _tdata[i+_toffset];
    }
    res.vlength = _getvi64len(res.data[0]) + (res.mlength - 1) * 9;
    delete[] _tdata;
    if(res.vlength>19) res.vi64 = 9223372036854775807;
    else if(res.mlength == 1) res.vi64 = res.data[0];
    else if(res.mlength == 2) res.vi64 = res.data[0] * S + res.data[1];
    else if(res.mlength == 3){
        res.vi64 = res.data[0] * S *S + res.data[1] * S + res.data[2];
        if(res.vi64 < 0)
            res.vi64 = 9223372036854775807;
    }
    return res;
}

/**
 * 取相反数, 直接取反negative就行
 */
bignum bignum::operator-() const{
    bignum res(*this);
    res.negative = !negative;
    return res;
}

bool bignum::operator<(const bignum& num) const{
    if(negative && !num.negative) return true;
    if(!negative && num.negative) return false;
    if(vlength != num.vlength)    return !negative == vlength < num.vlength;
    for(int i=0;i<mlength;i++)  
        if(data[i] != num.data[i]) return !negative == data[i] < num.data[i];
    return false;                                                                   // 相等
}

bool bignum::operator==(const bignum& num) const{
    if(negative != num.negative || vlength != num.vlength || vi64 != num.vi64) return false;
    for(int i = 0;i < mlength;i++)
        if(data[i] != num.data[i]) return false;
    return true;
}

bool bignum::operator>(const bignum& num) const{
    if(negative && !num.negative) return false;                 //  负数小于正数
    if(!negative && num.negative) return true;                  //  正数大于负数
    if(vlength != num.vlength)    return !negative == vlength > num.vlength;    //  正数更长的大
    for(int i = 0;i < mlength;i++)  
        if(data[i] != num.data[i]) return !negative == data[i] > num.data[i];   //  正数更大的大
    return false;                                               //  相等
}

bool bignum::operator<=(const bignum& num) const{
    if(negative && !num.negative) return true;
    if(!negative && num.negative) return false;
    if(vlength != num.vlength)    return !negative == vlength < num.vlength;
    for(int i=0;i<mlength;i++)  
        if(data[i] != num.data[i]) return !negative == data[i] < num.data[i];
    return true;                                                                    // 与小于号操作符唯一区别点, 前面代码都一样
}

bool bignum::operator!=(const bignum& num) const{
    return !(*this == num);
}

bool bignum::operator>=(const bignum& num) const{
    if(negative && !num.negative) return false;
    if(!negative && num.negative) return true;
    if(vlength != num.vlength)    return !negative == vlength > num.vlength;
    for(int i = 0;i < mlength;i++)  
        if(data[i] != num.data[i]) return !negative == data[i] > num.data[i];
    return true; 
}

/**
 * 拷贝构造函数,将主要是复制data里的数据
 */
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

/**
 * 先留着吧,程序出问题的时候.sh看不到错在哪,留着方便调试
 */
// int main(){
//     bignum c1("-80538738812075974");
//     string c2 = c1;
//     cout << c1 << "  " << c2 << endl;
//     return 0;
// }
