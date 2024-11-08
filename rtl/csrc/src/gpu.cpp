#include "gpu.h"

gpu::gpu(uint32_t size):vmem(size){
    this->if_start_up=0;
}

gpu::~gpu(){pthread_join(thread, nullptr);}

void* gpu::thread_function(void* arg) {
    gpu* gput = static_cast<gpu*>(arg);
    Draw2d* draw;
    TGAColor color;
    while(1){
        if(gput->get4B(0)==2)
        {
            uint32_t x0=gput->get4B(0x00800001);
            uint32_t y0=gput->get4B(0x00800005);
            uint32_t x1=gput->get4B(0x00800009);
            uint32_t y1=gput->get4B(0x0080000D);
            color.bgra[0]=gput->getB(0x00800011);
            color.bgra[1]=gput->getB(0x00800012);
            color.bgra[2]=gput->getB(0x00800013);
            color.bgra[3]=gput->getB(0x00800014);
            draw->line(x0,y0,x1,y1,color);
            gput->put4B(0,0);
        }
        else if(gput->get4B(0)==3)
        {
            uint32_t cx=gput->get4B(0x00800001);
            uint32_t cy=gput->get4B(0x00800005);
            uint32_t radius=gput->get4B(0x00800009);
            color.bgra[0]=gput->getB(0x0080000D);
            color.bgra[1]=gput->getB(0x0080000E);
            color.bgra[2]=gput->getB(0x0080000F);
            color.bgra[3]=gput->getB(0x00800010);
            draw->circle(cx,cy,radius,color);
            gput->put4B(0,0);
        }
        else if(gput->get4B(0)==4)
        {
            vec2 v0,v1,v2;
            v0.x=gput->get4B(0x00800001);
            v0.y=gput->get4B(0x00800005);
            v1.x=gput->get4B(0x00800009);
            v1.y=gput->get4B(0x0080000D);
            v2.x=gput->get4B(0x00800011);
            v2.y=gput->get4B(0x00800015);
            color.bgra[0]=gput->getB(0x00800019);
            color.bgra[1]=gput->getB(0x0080001A);
            color.bgra[2]=gput->getB(0x0080001B);
            color.bgra[3]=gput->getB(0x0080001C);
            draw->fillcolor(v0,v1,v2,color);
            gput->put4B(0,0);
        }
        else if(gput->get4B(0)==5)
        {
            uint32_t cx=gput->get4B(0x00800001);
            uint32_t cy=gput->get4B(0x00800005);
            uint32_t radius=gput->get4B(0x00800009);
            color.bgra[0]=gput->getB(0x0080000D);
            color.bgra[1]=gput->getB(0x0080000E);
            color.bgra[2]=gput->getB(0x0080000F);
            color.bgra[3]=gput->getB(0x00800010);
            draw->fillcolor(cx,cy,radius,color);
            gput->put4B(0,0);
        }
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