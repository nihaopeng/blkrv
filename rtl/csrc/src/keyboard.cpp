#ifdef ENABLE_GPU
    #include "keyboard.h"
    keyboard::keyboard(uint32_t size):vmem(size){
        this->putB(0,0);
        this->is_active=0;
        this->irq_pending=false;
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
    uint32_t keyboard::read(uint32_t offset, uint8_t op_type){
        // GPU 模式: 按键由 FLTK 回调写入数据寄存器, 走原 vmem 路径
        return vmem::read(offset, op_type);
    }
#else
    #include "keyboard.h"
    keyboard::keyboard(uint32_t size):vmem(size){
        this->putB(0,0);
        this->irq_pending=false;
    }
    keyboard::~keyboard(){
    }
    int keyboard::process(Bus* bus,uint32_t tick){
        // 把 stdin 中待读按键全部收入 FIFO (最多每 tick 3 字节), 不覆盖、不丢失
        uint32_t ch_int=utils::kbhit();
        if(ch_int & 0x000000ff) this->pending.push_back((uint8_t)(ch_int & 0x000000ff));
        if(ch_int & 0x0000ff00) this->pending.push_back((uint8_t)((ch_int>>8) & 0xff));
        if(ch_int & 0x00ff0000) this->pending.push_back((uint8_t)((ch_int>>16) & 0xff));
        // IRQ 拉高后保持, 直到 CPU 读走数据寄存器 (见 read) 才允许回落
        if(!this->pending.empty() && !this->irq_pending){
            bus->set_irq(2,true);
            this->irq_pending=true;
        }
        return 0;
    }
    uint32_t keyboard::read(uint32_t offset, uint8_t op_type){
        if(offset==0 && op_type==2){
            // 一次最多弹出 3 字节 (对应 OS 驱动的 3 个字节通道), 与老接口兼容
            uint32_t v=0;
            for(int i=0;i<3 && !this->pending.empty();i++){
                v |= ((uint32_t)this->pending.front()) << (8*i);
                this->pending.pop_front();
            }
            if(this->pending.empty() && this->irq_pending){
                this->irq_pending=false;
                if(this->bus) this->bus->set_irq(2,false);
            }
            return v;
        }
        return vmem::read(offset, op_type);
    }
#endif // DEBUG
