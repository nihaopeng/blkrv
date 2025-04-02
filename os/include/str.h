#ifndef _STR_H_
#define _STR_H_
#include "ini.h"
#include "math.h"
#include "drivers.h"

uint32_t str_len(const char* str);

int str_cmp(const char* str1,const char* str2);

void str_cpy(const char* src,char* dst);

void str_cpy_s(const char* src,char* dst,uint32_t start,uint32_t end);

void split(const char* str,char separator,uint32_t* node);

void itoa(int num, char *str);

void xtoa(int num, char *str);

void ftoa(float num, char *str, int precision);

int atoi(const char* str);

void uint32_to_char(uint32_t value, char *buf);

#endif // !_STR_H_
