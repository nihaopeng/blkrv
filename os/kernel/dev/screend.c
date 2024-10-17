#include "drivers.h"

int vprint_i(char* str,uint32_t length){
    for(uint32_t i=0;i<length;i++){
        char ch=*(str+i);
        char* addr=(char*)SCREEN_CACHE1_ADDR+i;
        memset_i(addr,ch);
    }
    char* ctrl_addr=(char*)SCREEN_CTRL_ADDR+3;
    memset_i(ctrl_addr,1);
    return 0;
}
