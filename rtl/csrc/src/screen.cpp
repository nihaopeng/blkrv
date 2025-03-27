#include "screen.h"

screen::screen(uint32_t size):vmem(size){

}

screen::~screen(){   
}

int screen::process(rib* rib,uint32_t tick){
    if(rib->s3_req){
        if(rib->s3_we){
            switch(rib->s3_mem_op_type){
                case 0:this->putB(rib->s3_addr,uint8_t(rib->s3_write_data));break;
                case 1:this->put2B(rib->s3_addr,uint16_t(rib->s3_write_data));break;
                case 2:this->put4B(rib->s3_addr,uint32_t(rib->s3_write_data));break;
                default:break;
            }
        }else{
            switch(rib->s3_mem_op_type){
                case 0:rib->s3_read_data=uint8_t(this->getB(rib->s3_addr));break;
                case 1:rib->s3_read_data=uint16_t(this->get2B(rib->s3_addr));break;
                case 2:rib->s3_read_data=uint32_t(this->get4B(rib->s3_addr));break;
                default:break;
            }
        }
    }
    //show content
    if(this->getB(3)==1){
        // printf("t;");
        // printf("%x%x;\n",this->get4B(4),this->get4B(8));
        for(int i=4;;i++){
            char ch=this->getB(i);
            this->putB(i,0);
            if(ch=='\0'){
                break;
            }
            printf("%c",ch);
        }
        this->putB(3,0);
    }
    return 0;
}