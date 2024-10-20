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