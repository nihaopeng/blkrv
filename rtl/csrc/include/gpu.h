#ifdef ENABLE_GPU

    #ifndef _GPU_H_
    #define _GPU_H_
    #include "vmem.h"
    #include "fltk2d.h"
    #include <atomic>

    #define GPU_ADDR_CACHE 0x00000000

    class gpu:public vmem
    {
    public:
        int if_start_up;
        int if_clear;
        Fl_Window* win;
        BufferedWidget* buffered_widget;
        pthread_t thread;
        gpu(uint32_t size);
        ~gpu();
        int process(rib* rib,uint32_t tick=0);
        static void draw(void* arg);
        static void* thread_function(void* arg);
    };

    #endif // !_GPU_H_

#endif // !ENABLE_GPU

