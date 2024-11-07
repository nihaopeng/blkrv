#include "drivers.h"

uint32_t rData_flag=0;

int send(socket sock,char* buf,uint32_t buf_length){
    uint32_t message_len=str_len(buf);
    
    rData_flag=0;
    str_cpy(sock.ip,(char*)NIC_IP_ADDR);
    *(uint32_t*)NIC_PORT_ADDR=sock.target_port;
    str_cpy(buf,(char*)NIC_SDATA_ADDR);
    *(uint32_t*)NIC_SDATA_LEN_ADDR=message_len;//put at last;
}

int recv(socket sock,char* buf,uint32_t buf_length,int* status){
    int rData_ptr=0;
    for(int i=0;i<100000;i++){
    // while(1){
        rData_ptr=*(int*)NIC_RDATA_LEN_ADDR;
        if(rData_flag<rData_ptr){
            for(int i=0;rData_flag<rData_ptr&&i<buf_length;i++,rData_flag++){
                buf[i]=*((char*)NIC_RDATA_ADDR+rData_flag);
            }
            printk("%d\r",rData_flag);
            *status=0;
            return 0;
        }
    }
    *status=-1;
    printk("time out\n");
}