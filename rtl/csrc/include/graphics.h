#ifdef ENABLE_GPU
//TODO:增加通用并行计算能力
    #ifndef _GPU_H_
    #define _GPU_H_
    #include "vmem.h"
    #include "fltk2d.h"
    #include "keyboard.h"
    #include "rib.h"
    #include <atomic>

    #define GPU_ADDR_CACHE 0x00000000
    #define INODE_START 0x00200000//存放inode，2MB大小，最多14979个文件。
    #define FAT_START 0x00400000//2MB,flash起始为0x40200000,前2MB为kernel程序存放位置,从0x40400000开始的2MB存储fat表
    #define DATA_START 0x00600000//4MB
    #define BLOCK_SIZE 1024//4KB
    #define EOF 0xFFFFFFFF
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
        rib* my_rib;
        keyboard* my_keyboard;
        MyWindow* win;
        BufferedWidget* buffered_widget;
        pthread_t thread;
        gpu(uint32_t size,keyboard* my_keyboard);
        ~gpu();
        int process(rib* my_rib,uint32_t tick=0);
        static void draw(void* arg);
        static void* thread_function(void* arg);
    };

    #endif // !_GPU_H_

#endif // !ENABLE_GPU

