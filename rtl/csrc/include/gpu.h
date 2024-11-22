#ifndef _GPU_H_
#define _GPU_H_
#include "vmem.h"
#include "fltk2d.h"
#include <atomic>

#define GPU_ADDR_CACHE1 0x00000000
#define GPU_ADDR_CACHE2 0x00100000

class gpu:public vmem
{
public:
    int if_start_up;
    my_window* win;
    pthread_t thread;
    int cur_cache;
    gpu(uint32_t size);
    ~gpu();
    void process(Vtop* top);
    static void draw(void* arg);
    static void* thread_function(void* arg);
};

#endif // !_GPU_H_