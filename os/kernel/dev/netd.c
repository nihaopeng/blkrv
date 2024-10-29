#include "drivers.h"

int send(socket sock,char* buf,uint32_t buf_length){
    char ip[]="127.0.0.1";
    uint32_t port=8080;
    char message[]="http/get:1.0";
    uint32_t message_len=str_len(message);
    
    str_cpy(ip,(char*)NIC_IP_ADDR);
    *(uint32_t*)NIC_PORT_ADDR=port;
    str_cpy(message,(char*)NIC_SDATA_ADDR);
    *(uint32_t*)NIC_SDATA_LEN_ADDR=message_len;//put at last;
}

int recv(socket sock,char* buf,uint32_t buf_length){
    uint32_t rData_len=0;
    while(1){
        rData_len=*(int*)NIC_RDATA_LEN_ADDR;
        if(rData_len){
            printk("%s",(char*)NIC_RDATA_ADDR);
            break;
        }
    }
}