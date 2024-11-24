#ifdef ENABLE_GPU

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
        char* t;int x0,y0,x1,y1,x2,y2,font,r,g,b;
        gput->buffered_widget->img_surf->set_current();  // 切换到缓冲区上下文
        if(gput->if_clear==0){
            gput->buffered_widget->clear_screen();
            gput->if_clear=1;
        }
        switch (event)
        {
        case 1:
            x0=gput->get4B(addr+4);y0=gput->get4B(addr+8);
            r=gput->get4B(addr+12);g=gput->get4B(addr+16);b=gput->get4B(addr+20);
            font=gput->get4B(addr+24);
            t=(char*)(gput->mem_space+28);
            gput->put4B(addr,0);
            gput->buffered_widget->text(x0,y0,r,g,b,font,t);
            break;
        case 2:
            x0=gput->get4B(addr+4);y0=gput->get4B(addr+8);
            x1=gput->get4B(addr+12);y1=gput->get4B(addr+16);
            x2=gput->get4B(addr+20);y2=gput->get4B(addr+24);
            r=gput->get4B(addr+28);g=gput->get4B(addr+32);b=gput->get4B(addr+36);
            gput->put4B(addr,0);
            gput->buffered_widget->triangle(x0,y0,x1,y1,x2,y2,r,g,b);
            break;
        case 3:
            gput->if_clear=0;
            gput->put4B(addr,0);
            gput->buffered_widget->flush();
        default:
            break;
        }
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

#endif // ENABLE_GPU