#include "mm.h"

int free_i(void* pointer){

}

void* malloc_i(uint32_t size){

}

_regist_syscall(void,free);

_regist_syscall(void,malloc);

int memset_s(char* addr,char ch,uint32_t count){
    for(int i=0;i<count;i++){
        *(addr) = ch;
        addr+=1;
    }
    return 0;
}