#ifdef ENABLE_GPU

    #include "graphics.h"
    #include <pthread.h>
    #include <fstream>

    gpu::gpu(uint32_t size,keyboard* my_keyboard):vmem(size){
        this->if_start_up=0;
        this->if_clear=0;
        this->my_keyboard=my_keyboard;
        this->buffered_widget=nullptr;
    }

    gpu::~gpu(){
        // 窗口线程可能从未被惰性拉起 (没有程序绘制过), 此时不能 join 未初始化的 thread
        if(this->if_start_up) pthread_join(thread, nullptr);
    }

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

    // 轮询 GPU 命令寄存器, 在离屏 RenderTexture 上执行绘制原语
    void gpu::draw(void* arg){
        gpu* gput=static_cast<gpu*>(arg);
        //get event
        uint32_t addr=GPU_ADDR_CACHE;
        uint32_t event=gput->get4B(addr);
        // char* t=new char[1024];
        char* t;unsigned char* image_buf;int x0,y0,x1,y1,x2,y2,font,r,g,b,x,y,image_size,inode_id;
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
            gput->buffered_widget->present();   // 帧完成: 提交完整帧到 front, 消除闪烁
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
            delete[] image_buf;
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
            delete[] image_buf;
            break;
        default:
            break;
        }
    }

    // raylib 窗口线程: 创建窗口 -> 500Hz 轮询 GPU 命令 + 呈现离屏缓冲
    void* gpu::thread_function(void* arg) {
        printf("gpu start up!\n");
        gpu* gput = static_cast<gpu*>(arg);

        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BLKRv GPU");
        if (!IsWindowReady()) {                             // 无显示环境 (如 docker/headless) 下优雅退出
            printf("gpu: raylib window init failed (no display?), GPU disabled\n");
            return nullptr;
        }
        SetExitKey(0);                                      // 禁用 raylib 默认 ESC 关窗, ESC 作为按键交给 OS
        SetTargetFPS(500);                                  // 与原 FLTK 500Hz 轮询一致
        gput->buffered_widget = new RayBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);

        while (!WindowShouldClose()) {
            poll_keyboard(gput->my_keyboard, gput->my_bus); // 键盘事件 -> 键盘设备
            gpu::draw(gput);                                // 轮询 GPU 命令并绘制

            BeginDrawing();
            ClearBackground(BLACK);
            // 呈现 front buffer (仅 flush 时更新的完整帧, source 高度取负翻转 Y 轴)
            DrawTextureRec(gput->buffered_widget->front.texture,
                           (Rectangle){ 0, 0, (float)SCREEN_WIDTH, -(float)SCREEN_HEIGHT },
                           (Vector2){ 0, 0 }, WHITE);
            EndDrawing();
        }

        delete gput->buffered_widget;
        gput->buffered_widget = nullptr;
        CloseWindow();
        return nullptr;
    }

    int gpu::process(Bus* bus,uint32_t tick){
        // 窗口不再随模拟器启动而打开: 只有用户程序第一次写 GPU 设备
        // (发绘制命令, 例如 snake/ppt_player) 时, 才由 write() 惰性拉起窗口线程
        (void)bus; (void)tick;
        return 0;
    }

    // 惰性拉窗: 首次写 GPU 设备时启动 raylib 窗口线程
    void gpu::ensure_window_started(){
        if(this->if_start_up) return;
        this->my_bus = this->bus;               // 总线指针由 register_dev 设置
        pthread_create(&thread,NULL,thread_function,this);
        this->if_start_up=1;
    }

    void gpu::write(uint32_t offset, uint32_t data, uint8_t op_type){
        ensure_window_started();                // 第一次发绘制命令时才建窗
        vmem::write(offset, data, op_type);
    }

#endif // ENABLE_GPU
