#ifdef ENABLE_GPU
    #include "keyboard.h"
    keyboard::keyboard(uint32_t size):vmem(size){
        this->putB(0,0);
        this->is_active=0;
    }
    keyboard::~keyboard(){
    }
    int keyboard::process(rib* rib,uint32_t tick){
        uint32_t tmp_is_active=this->is_active;
        this->is_active=(is_active==0&&rib->int_port2)?1:0;
        rib->int_port2=(tmp_is_active==0&&rib->int_port2)?1:0;
        rib->s2_read_valid=rib->int_port2;
        if(rib->s2_req){
            if(rib->s2_we){
                switch(rib->s2_mem_op_type){
                    case 0:this->putB(rib->s2_addr,uint8_t(rib->s2_write_data));break;
                    case 1:this->put2B(rib->s2_addr,uint16_t(rib->s2_write_data));break;
                    case 2:this->put4B(rib->s2_addr,uint32_t(rib->s2_write_data));break;
                    default:break;
                }
                printf("keyboard write:%x:%x\n",rib->s2_addr,rib->s2_write_data);
            }else{
                switch(rib->s2_mem_op_type){
                    case 0:rib->s2_read_data=uint8_t(this->getB(rib->s2_addr));break;
                    case 1:rib->s2_read_data=uint16_t(this->get2B(rib->s2_addr));break;
                    case 2:rib->s2_read_data=uint32_t(this->get4B(rib->s2_addr));break;
                    default:break;
                }
            }
        }
        return 0;
    }
#else
    #include "keyboard.h"
    keyboard::keyboard(uint32_t size):vmem(size){
        this->putB(0,0);
    }
    keyboard::~keyboard(){
    }
    int keyboard::process(rib* rib,uint32_t tick){
        rib->int_port2=0;
        rib->s2_read_valid=0;
        uint32_t ch_int=utils::kbhit();
        if(ch_int){
            // printf("%x\n",ch_int);
            rib->int_port2=1;
            this->put4B(0,ch_int);
            rib->s2_read_valid=1;
        }
        if(rib->s2_req){
            if(rib->s2_we){
                switch(rib->s2_mem_op_type){
                    case 0:this->putB(rib->s2_addr,uint8_t(rib->s2_write_data));break;
                    case 1:this->put2B(rib->s2_addr,uint16_t(rib->s2_write_data));break;
                    case 2:this->put4B(rib->s2_addr,uint32_t(rib->s2_write_data));break;
                    default:break;
                }
            }else{
                switch(rib->s2_mem_op_type){
                    case 0:rib->s2_read_data=uint8_t(this->getB(rib->s2_addr));break;
                    case 1:rib->s2_read_data=uint16_t(this->get2B(rib->s2_addr));break;
                    case 2:rib->s2_read_data=uint32_t(this->get4B(rib->s2_addr));break;
                    default:break;
                }
                // printf("%d",rib->s2_read_data);
            }
        }
        return 0;
    }
#endif // DEBUG