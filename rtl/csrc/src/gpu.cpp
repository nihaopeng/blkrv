#include "gpu.h"

gpu::gpu(uint32_t size):vmem(size){
    this->if_start_up=0;
    this->if_clear=0;
}

gpu::~gpu(){pthread_join(thread, nullptr);}

void gpu::draw(void* arg){
    gpu* gput=static_cast<gpu*>(arg);
    //get event
    uint32_t addr=GPU_ADDR_CACHE;
    uint32_t event=gput->get4B(addr);
    // char* t=new char[1024];
    char* t;
    gput->buffered_widget->img_surf->set_current();  // 切换到缓冲区上下文
    if(gput->if_clear==0){
        gput->buffered_widget->clear_screen();
        gput->if_clear=1;
    }
    switch (event)
    {
    case 1:
        // for(int i=0;;i++){
        //     char ch=(char)gput->getB(addr+28+i);
        //     gput->putB(addr+28+i,0);
        //     if(ch==0)break;
        //     t[i]=ch;
        // }
        t=(char*)(gput->mem_space+28);
        // printf("test0:%s,r:%d,g:%d,b:%d,font:%d,x0:%d,y0:%d\n",
            // t,gput->get4B(12),gput->get4B(16),gput->get4B(20),gput->get4B(24),gput->get4B(4),gput->get4B(8));
        gput->buffered_widget->text(
            gput->get4B(addr+4),gput->get4B(addr+8),
            gput->get4B(addr+12),gput->get4B(addr+16),gput->get4B(addr+20),
            gput->get4B(addr+24),
            t
        );
        // gput->win->text->redraw();
        break;
    case 2:
        gput->buffered_widget->triangle(
            gput->get4B(addr+4),gput->get4B(addr+8),
            gput->get4B(addr+12),gput->get4B(addr+16),
            gput->get4B(addr+20),gput->get4B(addr+24),
            gput->get4B(addr+28),gput->get4B(addr+32),gput->get4B(addr+36)
        );
        // gput->win->triangle->redraw();
        break;
    case 3:
        gput->if_clear=0;
        gput->buffered_widget->flush();
    default:
        break;
    }
    gput->put4B(addr,0);
    Fl::repeat_timeout(1/500,gpu::draw,arg);
}

void* gpu::thread_function(void* arg) {
    printf("gpu start up!\n");
    gpu* gput = static_cast<gpu*>(arg);
    gput->win = new Fl_Window(800, 600, "Rotating Triangle Example");
    gput->buffered_widget = new BufferedWidget(0, 0, 800, 600);
    gput->win->end();
    Fl::add_timeout(1/500,gpu::draw,arg);
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