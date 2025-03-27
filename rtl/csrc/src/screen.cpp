#include "screen.h"

screen::screen(uint32_t size):vmem(size){

}

screen::~screen(){   
}

void screen::process(rib* top,uint32_t tick){
    if(top->s3_req){
        if(top->s3_we){
            switch(top->s3_mem_op_type){
                case 0:this->putB(top->s3_addr,uint8_t(top->s3_write_data));break;
                case 1:this->put2B(top->s3_addr,uint16_t(top->s3_write_data));break;
                case 2:this->put4B(top->s3_addr,uint32_t(top->s3_write_data));break;
                default:break;
            }
        }else{
            switch(top->s3_mem_op_type){
                case 0:top->s3_read_data=uint8_t(this->getB(top->s3_addr));break;
                case 1:top->s3_read_data=uint16_t(this->get2B(top->s3_addr));break;
                case 2:top->s3_read_data=uint32_t(this->get4B(top->s3_addr));break;
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
}