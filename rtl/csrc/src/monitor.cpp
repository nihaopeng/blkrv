#include "monitor.h"

monitor::monitor(uint32_t size,std::string data_path){
    this->size=size;
    this->mem_space=(uint8_t*)malloc(sizeof(uint8_t)*size);
    this->data_path=data_path;
    // this->fp<<
    // "lui,auipc,jal,jalr,bj,load,store,calc,calci,sys,bios,ram,keyboard,screen,nic,flash"
    // <<std::endl;
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

int monitor::process(rib* rib,mmu* my_mmu,uint32_t tick){
    if(rib->s8_req){
        if(rib->s8_we){
            switch(rib->s8_mem_op_type){
                case 0:this->putB(rib->s8_addr,uint8_t(rib->s8_write_data));break;
                case 1:this->put2B(rib->s8_addr,uint16_t(rib->s8_write_data));break;
                case 2:this->put4B(rib->s8_addr,uint32_t(rib->s8_write_data));break;
                default:break;
            }
        }else{
            switch(rib->s8_mem_op_type){
                case 0:rib->s8_read_data=uint8_t(this->getB(rib->s8_addr));break;
                case 1:rib->s8_read_data=uint16_t(this->get2B(rib->s8_addr));break;
                case 2:rib->s8_read_data=uint32_t(this->get4B(rib->s8_addr));break;
                default:break;
            }
            // printf("%d",rib->s2_read_data);
        }
    }
    // if(this->is_enable=0)
    if(this->get4B(0)==1)//TODO:传输写入的文件。
    {
        // printf("monitor opened\n");
        // inst_type={22'd0,lui,auipc,jal,jalr,bj,load,store,calc,calci,sys};
        if(this->is_enable==0){
            this->start_time = std::chrono::high_resolution_clock::now();
        }
        this->is_enable=1;
        uint32_t inst_type=rib->inst_type_o;
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
        this->s0_req+=rib->s0_req;
        this->s1_req+=rib->s1_req;
        this->s2_req+=rib->s2_req;
        this->s3_req+=rib->s3_req;
        this->s4_req+=rib->s4_req;
        this->s5_req+=rib->s5_req;
        this->s6_req+=rib->s6_req;
        if(((inst_type&(0x0000001f))>>4)&&rib->s1_addr%4==0){
            this->align_times+=1;
        }else if(((inst_type&(0x0000001f))>>4)){
            this->none_align_times+=1;
        }
        this->hit_times+=my_mmu->is_hit;
        this->mmu_enable_times+=my_mmu->is_enable;
    }else if(this->is_enable==1&&this->get4B(0)==0){//开启后又关闭
        // printf("physic monitor opened\n");
        this->fp.open(this->data_path,std::ios::out);
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