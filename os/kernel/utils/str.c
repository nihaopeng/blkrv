#include "str.h"

uint32_t str_len(const char* str){
    uint32_t i=0;
    while(str[i]!='\0')
        i++;
    return i;
}

int str_cmp(const char* str1,const char* str2){//same return 1,else return 0;
    uint32_t i=0;
    if(str_len(str1)!=str_len(str2))
        return 0;
    while(str1[i]!='\0'&&str2[i]!='\0'){
        if(str1[i]!=str2[i]){
            return 0;
        }
        i++;
    }
    return 1;
}

void str_cpy(const char* src,char* dst){//
    uint32_t data_len=str_len(src);
    for(uint32_t i=0;i<data_len;i++){
        dst[i]=src[i];
    }
}

void str_cpy_s(const char* src,char* dst,uint32_t start,uint32_t end){//[]
    // uint32_t data_len=str_len(src);
    for(uint32_t i=start;i<=end;i++){
        dst[i]=src[i];
    }
}



void split(const char* str,char separator,uint32_t* node){
    uint32_t length=str_len(str);
    uint32_t pos=0;
    for(uint32_t i=0;i<length;i++){
        if(str[i]==separator){
            node[pos++]=i;
        }
    }    
}

void itoa(int num, char *str) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    int isNegative = num < 0;
    int n = isNegative ? -num : num;
    // Generate the string from the digits
    int pos=0;
    while (n != 0) {
        int temp = mod(n , 10);  // Get the last digit
        str[pos++] = temp + '0';  // Convert the digit to a character
        n = div(n , 10);  // Remove the last digit
    }
    if (isNegative) {
        str[pos++] = '-';
    }
    str[pos] = '\0';  // Null terminate the string
    for (int i = 0; i < div(pos , 2); i++) {
        char temp = str[i];
        str[i] = str[pos - i - 1];
        str[pos - i - 1] = temp;
    }
}

void xtoa(int num, char *str) {
    char buffer[17]; // 缓冲区：16位十六进制 + 终止符
    int is_negative = 0;
    uint32_t unum;
    // 处理0的特殊情况
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    // 处理负数
    int n = (int)num; // 兼容32/64位系统
    if (n < 0) {
        is_negative = 1;
        unum = (uint32_t)(-n); // 转为正数并避免溢出
    } else {
        unum = (uint32_t)n;
    }
    // 从后往前填充十六进制字符
    int index = sizeof(buffer) - 1;
    buffer[index--] = '\0'; // 终止符
    const char digits[] = "0123456789ABCDEF";
    do {
        buffer[index--] = digits[unum % 16];
        unum /= 16;
    } while (unum > 0);
    // 添加负号
    if (is_negative) {
        buffer[index--] = '-';
    }
    // 将结果复制到调用者提供的字符串
    char *src = buffer + index + 1;
    while ((*str++ = *src++) != '\0');
}

// void ftoa(float num, char *buffer, int precision) {
//     // 处理符号
//     int is_negative = 0;
//     if (num < 0) {
//         is_negative = 1;
//         num = -num;
//     }

//     // 分解整数和小数部分
//     int integer_part = (int)num;
//     float decimal_part = num - (float)integer_part;

//     // 转换为整数部分的字符串
//     char int_buffer[16];
//     int i = 0;
//     do {
//         int_buffer[i++] = (integer_part % 10) + '0';
//         integer_part /= 10;
//     } while (integer_part > 0);

//     // 处理负数并反转整数部分字符串
//     int j = 0;
//     if (is_negative) buffer[j++] = '-';
//     while (i > 0) buffer[j++] = int_buffer[--i];
//     buffer[j++] = '.'; // 添加小数点

//     // 转换为小数部分的字符串
//     for (int k = 0; k < precision; k++) {
//         decimal_part *= 10;
//         int digit = (int)decimal_part;
//         printk("digit:%d,",digit);
//         buffer[j++] = digit + '0';
//         decimal_part -= digit;
//     }

//     buffer[j] = '\0'; // 终止字符串
// }

int atoi(const char* str){
    int res = 0;
    int sign = 1;
    while (*str==32) str++;
    if (*str == '+' || *str == '-') {
        sign = (*str == '-') ? -1 : 1;
        str++;
    }
	while (*str>='0'&&*str<='9') {
        res = mul(res , 10) + (*str - '0');
        str++;
    }
    if(res==INT_MIN){
    	return res;
	}
    return mul(sign , res);
}

void uint32_to_char(uint32_t value, char *buf) {
    buf[0] = value & 0xFF;
    buf[1] = (value >> 8) & 0xFF;
    buf[2] = (value >> 16) & 0xFF;
    buf[3] = (value >> 24) & 0xFF;
}