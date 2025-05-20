#ifdef ENABLE_GPU

    #include "graphics.h"
    gpu::gpu(uint32_t size,keyboard* my_keyboard):vmem(size){
        this->if_start_up=0;
        this->if_clear=0;
        this->my_keyboard=my_keyboard;
    }

    gpu::~gpu(){pthread_join(thread, nullptr);}

    uint32_t get_next_block_id_s(std::fstream &fp,uint32_t cur_block_id){
        fp.seekg(FAT_START+cur_block_id*4);
        uint32_t data;
        fp.read(reinterpret_cast<char*>(&data), sizeof(data));
        return data;
    }
    
    screen_inode* get_inode_by_id_s(std::fstream &fp,uint32_t inode_id){
        fp.seekg(INODE_START+inode_id*sizeof(screen_inode));
        screen_inode* data=new screen_inode;
        fp.read(reinterpret_cast<char*>(data), sizeof(screen_inode));
        return data;
    }
    
    char getB(std::fstream &fp,char* addr){
        fp.seekg((uint64_t)addr);
        char data;
        fp.read(reinterpret_cast<char*>(&data), sizeof(data));
        return data;
    }
    
    // 读取文件
    int readk_s(uint32_t inode_id,unsigned char* buf) {
        std::fstream f;
        f.open("./devices/flash",std::ios::binary|std::ios::in);
        screen_inode* f_inode=get_inode_by_id_s(f,inode_id);
        uint32_t block_id=f_inode->start_block;
        // printf("readk_s:inode_id:%d,block_id:%d,size:%d\n",inode_id,block_id,f_inode->size);
        if(block_id==EOF) return 0;
        char* fp=(char*)(DATA_START+block_id*BLOCK_SIZE);
        // printk("read:block_id:%d,block_addr:%x,fp:%x\n",block_id,block_addr,fp);
        for(int i=0;i<f_inode->size;i++){
            buf[i]=getB(f,fp);
            fp+=1;
            if(((uint64_t)fp-(uint64_t)DATA_START)%(uint64_t)BLOCK_SIZE==0){
                block_id=get_next_block_id_s(f,block_id);
                // printf("block_id:%d\n",block_id);
                if(block_id==EOF) return 0;
                fp=(char*)(DATA_START+block_id*BLOCK_SIZE);
            }
        }
        return 0;
    }

    void gpu::draw(void* arg){
        gpu* gput=static_cast<gpu*>(arg);
        //get event
        uint32_t addr=GPU_ADDR_CACHE;
        uint32_t event=gput->get4B(addr);
        // char* t=new char[1024];
        char* t;unsigned char* image_buf;int x0,y0,x1,y1,x2,y2,font,r,g,b,x,y,image_size,inode_id;
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
            // gput->buffered_widget->clear_screen();
            gput->put4B(addr,0);
            gput->buffered_widget->flush();
            break;
        case 4:
            x=gput->get4B(addr+4);
            y=gput->get4B(addr+8);
            image_size=gput->get4B(addr+12);
            inode_id=gput->get4B(addr+16);
            printf("x:%d,y:%d,size:%d,inode_id:%d\n",x,y,image_size,inode_id);
            gput->put4B(addr,0);
            image_buf=new unsigned char[image_size];
            readk_s(inode_id,image_buf);
            // printf("image_buf:%c%c%c%c\n",image_buf[0],image_buf[1],image_buf[2],image_buf[3]);
            gput->buffered_widget->draw_jpg(image_buf,image_size,x,y);
            break;
        case 5:
            x=gput->get4B(addr+4);
            y=gput->get4B(addr+8);
            image_size=gput->get4B(addr+12);
            inode_id=gput->get4B(addr+16);
            // printf("x:%d,y:%d,size:%d,inode_id:%d\n",x,y,image_size,inode_id);
            gput->put4B(addr,0);
            image_buf=new unsigned char[image_size];
            readk_s(inode_id,image_buf);
            // printf("image_buf:%c%c%c%c\n",image_buf[0],image_buf[1],image_buf[2],image_buf[3]);
            gput->buffered_widget->draw_png(image_buf,image_size,x,y);
            break;
        default:
            break;
        }
        Fl::repeat_timeout(1/500,gpu::draw,arg);
    }

    void* gpu::thread_function(void* arg) {
        printf("gpu start up!\n");
        gpu* gput = static_cast<gpu*>(arg);
        gput->win = new MyWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rotating Triangle Example");
        gput->buffered_widget = new BufferedWidget(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        gput->win->my_rib=gput->my_rib;
        gput->win->my_keyboard=gput->my_keyboard;
        Fl::add_timeout(1/500,gpu::draw,arg);
        gput->win->show();
        Fl::run();
        return nullptr;
    }

    int gpu::process(rib* my_rib,uint32_t tick){
        if(!this->if_start_up){
            this->my_rib=my_rib;
            pthread_create(&thread,NULL,thread_function,this);
            this->if_start_up=1;
        }
        if(my_rib->s4_req){
            if(my_rib->s4_we){
                switch(my_rib->s4_mem_op_type){
                    case 0:this->putB(my_rib->s4_addr,uint8_t(my_rib->s4_write_data));break;
                    case 1:this->put2B(my_rib->s4_addr,uint16_t(my_rib->s4_write_data));break;
                    case 2:this->put4B(my_rib->s4_addr,uint32_t(my_rib->s4_write_data));
                            // printf("%d:%d:%d\n",rib->s1_addr,rib->s1_write_data,this->get4B(rib->s1_addr));
                            break;
                    default:break;
                }
            }else{
                switch(my_rib->s4_mem_op_type){
                    case 0:my_rib->s4_read_data=uint8_t(this->getB(my_rib->s4_addr));break;
                    case 1:my_rib->s4_read_data=uint16_t(this->get2B(my_rib->s4_addr));break;
                    case 2:my_rib->s4_read_data=uint32_t(this->get4B(my_rib->s4_addr));break;
                    default:break;
                }
            }
        }
        return 0;
    }

#endif // ENABLE_GPU