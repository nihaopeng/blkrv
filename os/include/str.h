#ifndef _STR_H_
#define _STR_H_
#include "ini.h"
#include "math.h"
#include "drivers.h"

uint32_t str_len(const char* str);

int str_cmp(char* str1,char* str2);

void str_cpy(char* src,char* dst);

void str_cpy_s(char* src,char* dst,uint32_t start,uint32_t end);

void split(const char* str,char separator,uint32_t* node);

void itoa(int num, char *str);

int atoi(char* str);

#endif // !_STR_H_
