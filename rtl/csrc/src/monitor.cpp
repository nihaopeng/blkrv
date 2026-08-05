#include "monitor.h"
#include <cstdlib>
#include <cstdio>

monitor::monitor(uint32_t size,std::string data_path){
    this->size=size;
    this->mem_space=(uint8_t*)malloc(sizeof(uint8_t)*size);
    this->data_path=data_path;
    this->fp.open(this->data_path,std::ios::out | std::ios::trunc);  // 关键：使用 trunc 清空文件
}

monitor::~monitor(){
    this->fp.close();
}

uint8_t monitor::getB(uint32_t pointer){
    if(pointer>=this->size){
        // printf("\n%u out of mem %u\ncnt:%lu\n",pointer,this->size,main_time);
        // exit(-1);
        return 0;
    }
    return *((uint8_t*)(this->mem_space+pointer));
}
uint16_t monitor::get2B(uint32_t pointer){
    if(pointer>=this->size){
        // printf("\n%u out of mem %u\ncnt:%lu\n",pointer,this->size,main_time);
        // exit(-1);
        return 0;
    }
    return *((uint16_t*)(this->mem_space+pointer));
}
uint32_t monitor::get4B(uint32_t pointer){
    if(pointer>=this->size){
        // printf("\n%u out of mem %u\ncnt:%lu\n",pointer,this->size,main_time);
        // exit(-1);
        return 0;
    }
    return *((uint32_t*)(this->mem_space+pointer));
}
void monitor::putB(uint32_t pointer,uint8_t data){
    if(pointer>=this->size){
        // printf("\n%u out of mem %u\ncnt:%lu\n",pointer,this->size,main_time);
        // exit(-1);
        return;
    }
    *((uint8_t*)(this->mem_space+pointer))=data;
}
void monitor::put2B(uint32_t pointer,uint16_t data){
    if(pointer>=this->size){
        // printf("\n%u out of mem %u\ncnt:%lu\n",pointer,this->size,main_time);
        // exit(-1);
        return;
    }
    *((uint16_t*)(this->mem_space+pointer))=data;
}
void monitor::put4B(uint32_t pointer,uint32_t data){
    if(pointer>=this->size){
        // printf("\n%u out of mem %u\ncnt:%lu\n",pointer,this->size,main_time);
        // exit(-1);
        return;
    }
    *((uint32_t*)(this->mem_space+pointer))=data;
}

uint32_t monitor::read(uint32_t offset, uint8_t op_type){
    switch(op_type){
        case 0:return uint8_t(this->getB(offset));
        case 1:return uint16_t(this->get2B(offset));
        case 2:return uint32_t(this->get4B(offset));
        default:return 0;
    }
}

void monitor::write(uint32_t offset, uint32_t data, uint8_t op_type){
    switch(op_type){
        case 0:this->putB(offset,uint8_t(data));break;
        case 1:this->put2B(offset,uint16_t(data));break;
        case 2:this->put4B(offset,uint32_t(data));break;
        default:break;
    }
}

int monitor::process(Bus* bus,mmu* my_mmu,uint32_t tick,uint32_t inst_type){
    // if(this->is_enable=0)
    if(this->get4B(0)==1)//TODO:传输写入的文件。
    {
        // printf("monitor opened\n");
        // inst_type={22'd0,lui,auipc,jal,jalr,bj,load,store,calc,calci,sys};
        if(this->is_enable==0){
            this->start_time = std::chrono::high_resolution_clock::now();
        }
        this->is_enable=1;
        this->sum_ticks+=1;
        this->lui_times+=((inst_type&(0x000003ff))>>9);
        this->auipc_times+=((inst_type&(0x000001ff))>>8);
        this->jal_times+=((inst_type&(0x000000ff))>>7);
        this->jalr_times+=((inst_type&(0x0000007f))>>6);
        this->bj_times+=((inst_type&(0x0000003f))>>5);
        this->load_times+=((inst_type&(0x0000001f))>>4);
        this->store_times+=((inst_type&(0x0000000f))>>3);
        this->calc_times+=((inst_type&(0x00000007))>>2);
        this->calci_times+=((inst_type&(0x00000003))>>1);
        this->sys_times+=((inst_type&(0x00000001))>>0);
        this->s0_req+=bus->req_count[0];
        this->s1_req+=bus->req_count[1];
        this->s2_req+=bus->req_count[2];
        this->s3_req+=bus->req_count[3];
        this->s4_req+=bus->req_count[4];
        this->s5_req+=bus->req_count[5];
        this->s6_req+=bus->req_count[6];
        if(((inst_type&(0x0000001f))>>4)&&bus->last_addr%4==0){
            this->align_times+=1;
        }else if(((inst_type&(0x0000001f))>>4)){
            this->none_align_times+=1;
        }
        this->hit_times+=my_mmu->is_hit;
        this->mmu_enable_times+=my_mmu->is_enable;

        uint8_t flag=0;
        if(((inst_type&(0x0000001f))>>4)||((inst_type&(0x0000000f))>>3)){
            flag=flag|0b10000000;//mem op
        }
        if(my_mmu->is_enable){
            flag=flag|0b01000000;//mmu enable
        }
        if(((inst_type&(0x00000007))>>2)||((inst_type&(0x00000003))>>1)){
            flag=flag|0b00100000;//calc op
        }
        if(my_mmu->is_hit){
            flag=flag|0b00010000;//hit
        }
        this->fp<<this->sum_ticks<<":"<<(uint32_t)flag<<std::endl;
        this->fp.flush();
    }else if(this->is_enable==1&&this->get4B(0)==0){//开启后又关闭
        printf("physic monitor closed\n");
        this->end_time = std::chrono::high_resolution_clock::now();
        this->is_enable=0;
        this->fp<<"sum_ticks:"<<this->sum_ticks<<std::endl
                <<"lui_times:"<<this->lui_times<<std::endl
                <<"auipc_times:"<<this->auipc_times<<std::endl
                <<"jal_times:"<<this->jal_times<<std::endl
                <<"jalr_times:"<<this->jalr_times<<std::endl
                <<"bj_times:"<<this->bj_times<<std::endl
                <<"load_times:"<<this->load_times<<std::endl
                <<"store_times:"<<this->store_times<<std::endl
                <<"calc_times:"<<this->calc_times<<std::endl
                <<"calci_times:"<<this->calci_times<<std::endl
                <<"sys_times:"<<this->sys_times<<std::endl
                <<"s0_req:"<<this->s0_req<<std::endl
                <<"s1_req:"<<this->s1_req<<std::endl
                <<"s2_req:"<<this->s2_req<<std::endl
                <<"s3_req:"<<this->s3_req<<std::endl
                <<"s4_req:"<<this->s4_req<<std::endl
                <<"s5_req:"<<this->s5_req<<std::endl
                <<"s6_req:"<<this->s6_req<<std::endl
                <<"align_times:"<<this->align_times<<std::endl
                <<"none_align_times:"<<this->none_align_times<<std::endl
                <<"mmu_enable_times:"<<this->mmu_enable_times<<std::endl
                <<"hit_times:"<<this->hit_times<<std::endl
                <<"phy_time:"<<(std::chrono::duration_cast<std::chrono::milliseconds>)(this->end_time-this->start_time).count()<<" ms"<<std::endl;
            this->fp.flush();
            this->fp.close();
    }
    return 0;
}
