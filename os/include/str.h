#ifndef _STR_H_
#define _STR_H_
#include "ini.h"
#include "math.h"

uint32_t str_len(const char* str){
    uint32_t i=0;
    while(str[i]!='\0')
        i++;
    return i;
}

int str_cmp(char* str1,char* str2){//same return 1,else return 0;
    uint32_t i;
    if(str_len(str1)!=str_len(str2))
        return 0;
    while(str1[i]!='\0'&&str2[i]!='\0'){
        if(str1[i]!=str2[i]){
            return 0;
        }
    }
    return 1;
}

void str_cpy(char* src,char* dst){//
    uint32_t data_len=str_len(src);
    for(int i=0;i<data_len;i++){
        dst[i]=src[i];
    }
}

void str_cpy_s(char* src,char* dst,uint32_t start,uint32_t end){//[]
    // uint32_t data_len=str_len(src);
    for(int i=0;i<MAX_NAME;i++){
        dst[i]=0;
    }
    for(int i=start;i<=end;i++){
        dst[i]=src[i];
    }
}



void split(const char* str,char separator,uint32_t* node){
    uint32_t length=str_len(str);
    uint32_t pos=0;
    for(int i=0;i<length;i++){
        if(str[i]==separator){
            node[pos++]=i;
        }
    }    
}


void itoa(int num,char* str,int radix){
    int i = 0;
    int sum;
    uint32_t num1 = num;  //如果是负数求补码，必须将他的绝对值放在无符号位中在进行求反码
    char str1[33] = { 0 };
    if (num<0) {              //求出负数的补码
        num = -num;
        num1 = ~num;
        num1 += 1;
    }
    if (num == 0) {             
        str1[i] = '0';
        
        i++;
    }
    while(num1 !=0) {                      //进行进制运算
        sum = mod(num1 , radix);
        str1[i] = (sum > 9) ? (sum - 10) + 'a' : sum + '0';
        num1 = div(num1 , radix);
        i++;
    }
    i--;
    int j = 0;
    str[i+1]='\0';
    for (i; i >= 0; i--) {               //逆序输出 
        str[i] = str1[j];
        j++;
    }
}
#endif // !_STR_H_
