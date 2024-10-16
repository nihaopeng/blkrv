#ifndef _MATH_H_
#define _MATH_H_

int abs(int a){
    return a>0?a:-a;
}

int mod(int a,int b){
    // 处理特殊情况
    if (a == 0 || b == 0) return 0;
    if (b == 1) return 0;
    int result = a;
    int divisor = b;
    int quotient = 0;
    // 使用位移和减法来模拟除法
    if(a>0){
        while (result >= divisor) {
            int temp = divisor;
            int multiple = 1;
            while (((temp << 1) <= result)&&(temp << 1)<=0x40000000) {
                temp <<= 1;
                multiple <<= 1;
            }
            result -= temp;
            quotient += multiple;
        }
    }else{
        while (result <= -divisor) {
            int temp = -divisor;
            int multiple = 1;
            while (((temp << 1) >= result) && (temp << 1) >= -1073741824) {
                temp <<= 1;
                multiple <<= 1;
            }
            result -= temp;
            quotient -= multiple;
        }
    }
    
    // result 现在是 a 除以 b 的余数
    return result;
}

int div(int dividend,int divisor){
    // 处理特殊情况
    if (dividend == 0) return 0;
    // 确定结果的符号
    int negative = (dividend < 0) ^ (divisor < 0);
    int result = 0;
    // 将被除数和除数都转换为正数以简化计算
    dividend = abs(dividend);
    divisor = abs(divisor);
    // 使用位移和减法来实现除法
    while (dividend >= divisor) {
        int temp = divisor;
        int multiple = 1;
        while (((temp << 1) <= dividend)&&(temp << 1)<=0x40000000) {
            temp <<= 1;
            multiple <<= 1;
        }
        dividend -= temp;
        result += multiple;
    }
    // 如果结果应该是负数，则取反
    if (negative) {
        result = -result;
    }
    return result;

}

int mul(int a, int b) {
    int result = 0;
    int sign = 1;
    // 处理负数的情况
    if (a < 0) {
        sign = -sign;
        a = -a;
    }
    if (b < 0) {
        sign = -sign;
        b = -b;
    }
    // 将b转换为二进制形式，并逐位检查
    while (b > 0) {
        // 如果b的最低位为1，则将a加到结果中
        if (b & 1) {
            result += a;
        }

        // 将a左移一位（乘以2），并将b右移一位（除以2）
        a <<= 1;
        b >>= 1;
    }
    // 根据原始符号调整结果的符号
    return (sign==1)?result:-result;
}

#endif // !_MATH_H_
