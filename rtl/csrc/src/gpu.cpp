#include "gpu.h"

gpu::gpu(uint32_t size):vmem(size){
    this->if_start_up=0;
}

gpu::~gpu(){pthread_join(thread, nullptr);}

void* gpu::thread_function(void* arg) {
    gpu* gput = static_cast<gpu*>(arg);
    while(1){
        
    }
}

void gpu::process(Vtop* top){
    if(!this->if_start_up){
        pthread_create(&thread,NULL,thread_function,this);
        this->if_start_up=1;
    }
    if(top->s4_req){
        if(top->s4_we){
            switch(top->s4_mem_op_type){
                case 0:this->putB(top->s4_addr,uint8_t(top->s4_write_data));break;
                case 1:this->put2B(top->s4_addr,uint16_t(top->s4_write_data));break;
                case 2:this->put4B(top->s4_addr,uint32_t(top->s4_write_data));
                        // printf("%d:%d:%d\n",top->s1_addr,top->s1_write_data,this->get4B(top->s1_addr));
                        break;
                default:break;
            }
        }else{
            switch(top->s4_mem_op_type){
                case 0:top->s4_read_data=uint8_t(this->getB(top->s4_addr));break;
                case 1:top->s4_read_data=uint16_t(this->get2B(top->s4_addr));break;
                case 2:top->s4_read_data=uint32_t(this->get4B(top->s4_addr));break;
                default:break;
            }
        }
    }
}