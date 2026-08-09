#ifdef ENABLE_GPU
//TODO:增加通用并行计算能力
    #ifndef _GPU_H_
    #define _GPU_H_
    #include "vmem.h"
    #include "ray2d.h"
    #include "keyboard.h"
    #include "bus.h"
    #include <atomic>

    #define GPU_ADDR_CACHE 0x00000000
    #define INODE_START 0x00200000//存放inode，2MB大小，最多14979个文件。
    #define FAT_START 0x00400000//2MB,flash起始为0x40200000,前2MB为kernel程序存放位置,从0x40400000开始的2MB存储fat表
    #define DATA_START 0x00600000//4MB
    #define BLOCK_SIZE 1024//4KB
    #ifndef EOF
    #define EOF 0xFFFFFFFF
    #endif
    #define MAX_NAME 128

    #define SCREEN_WIDTH 1920
    #define SCREEN_HEIGHT 1080

    typedef struct screen_file{
        char file_name[MAX_NAME];
        uint32_t size;
        uint32_t start_block;
        uint8_t type;
    } screen_inode;

    class gpu:public vmem
    {
    public:
        int if_start_up;
        int if_clear;
        Bus* my_bus;
        keyboard* my_keyboard;
        RayBuffer* buffered_widget;
        pthread_t thread;
        gpu(uint32_t size,keyboard* my_keyboard);
        ~gpu();
        // 惰性拉窗: 只有用户程序第一次写 GPU 设备 (发绘制命令) 时才启动窗口线程
        void ensure_window_started();
        void write(uint32_t offset, uint32_t data, uint8_t op_type) override;
        // 每 tick 的周期回调 (不再在启动时建窗)
        int process(Bus* bus,uint32_t tick=0);
        static void draw(void* arg);
        static void* thread_function(void* arg);
    };

    #endif // !_GPU_H_

#endif // !ENABLE_GPU
