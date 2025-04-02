#include "net.h"

uint32_t rData_flag=0;

int init_net(){
    rData_flag=0;
}

int recvk(socket* sock,char* buf,uint32_t buf_length){
    int rData_ptr=0;
    for(int i=0;i<100000;i++){
        rData_ptr=*(int*)NIC_RDATA_LEN_ADDR;
        if(rData_flag<rData_ptr){
            // printk("%d\n",rData_flag);
            for(int i=0;rData_flag<rData_ptr&&i<buf_length;i++,rData_flag++){
                buf[i]=*((char*)NIC_RDATA_ADDR+rData_flag);
            }
            return 0;
        }
    }
    *(int*)NIC_RDATA_LEN_ADDR=0;//清空网卡缓存
    printk("time out\n");
    return -1;
}

int sendk(socket* sock,char* buf,uint32_t buf_length){
    uint32_t message_len=str_len(buf);
    rData_flag=0;
    str_cpy(sock->ip,(char*)NIC_IP_ADDR);
    *(uint32_t*)NIC_PORT_ADDR=sock->target_port;
    str_cpy(buf,(char*)NIC_SDATA_ADDR);
    *(uint32_t*)NIC_SDATA_LEN_ADDR=message_len;//put at last;
}