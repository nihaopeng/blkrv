#include "gpu.h"

gpu::gpu(uint32_t size):vmem(size){
    this->if_start_up=0;
}

gpu::~gpu(){pthread_join(thread, nullptr);}

void gpu::draw(void* arg){
    gpu* gput=static_cast<gpu*>(arg);
    //get event
    uint32_t addr=(gput->get4B(GPU_ADDR_FREE)==2)?GPU_ADDR_CACHE1:GPU_ADDR_CACHE2;
    gput->put4B(GPU_ADDR_FREE,(gput->get4B(GPU_ADDR_FREE)==2)?1:2);
    uint32_t event=gput->get4B(addr);
    char* t=new char[1024];
    switch (event)
    {
    case 1:
        // printf("draw text\n");
        for(int i=0;;i++){
            char ch=(char)gput->getB(addr+28+i);
            gput->putB(addr+28+i,0);
            if(ch==0)break;
            t[i]=ch;
        }
        // printf("test0:%s,r:%d,g:%d,b:%d,font:%d,x0:%d,y0:%d\n",
            // t,gput->get4B(12),gput->get4B(16),gput->get4B(20),gput->get4B(24),gput->get4B(4),gput->get4B(8));
        gput->win->text->add(
            gput->get4B(addr+4),gput->get4B(addr+8),
            gput->get4B(addr+12),gput->get4B(addr+16),gput->get4B(addr+20),
            gput->get4B(addr+24),
            t
        );
        break;
    case 2:
        gput->win->triangle->add(
            gput->get4B(addr+4),gput->get4B(addr+8),
            gput->get4B(addr+12),gput->get4B(addr+16),
            gput->get4B(addr+20),gput->get4B(addr+24),
            gput->get4B(addr+28),gput->get4B(addr+32),gput->get4B(addr+36)
        );
        break;
    case 3:
        // printf("flush\n");
        gput->win->clearw->flush();
    default:
        break;
    }
    gput->put4B(addr,0);
    Fl::repeat_timeout(1/60,draw,arg);
}

void* gpu::thread_function(void* arg) {
    printf("gpu start up!\n");
    gpu* gput = static_cast<gpu*>(arg);
    gput->win=new my_window(800,600,"my window");
    Fl::add_timeout(1/60,draw,arg);
    gput->win->show();
    Fl::run();
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