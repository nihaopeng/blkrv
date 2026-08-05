#ifdef ENABLE_GPU
    #include "keyboard.h"
    keyboard::keyboard(uint32_t size):vmem(size){
        this->putB(0,0);
        this->is_active=0;
    }
    keyboard::~keyboard(){
    }
    int keyboard::process(Bus* bus,uint32_t tick){
        // GPU 模式下按键由 FLTK 窗口回调设置, 这里锁存一拍后复位
        uint32_t tmp_is_active=this->is_active;
        bool irq=bus->get_irq(2);
        this->is_active=(is_active==0&&irq)?1:0;
        bus->set_irq(2,(tmp_is_active==0&&irq)?1:0);
        return 0;
    }
#else
    #include "keyboard.h"
    keyboard::keyboard(uint32_t size):vmem(size){
        this->putB(0,0);
    }
    keyboard::~keyboard(){
    }
    int keyboard::process(Bus* bus,uint32_t tick){
        bus->set_irq(2,false);
        uint32_t ch_int=utils::kbhit();
        if(ch_int){
            bus->set_irq(2,true); //
            this->put4B(0,ch_int);
        }
        return 0;
    }
#endif // DEBUG
