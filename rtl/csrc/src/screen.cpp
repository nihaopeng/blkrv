#include "screen.h"
#include <cstdio>

screen::screen(uint32_t size):vmem(size){

}

screen::~screen(){
}

int screen::process(Bus* bus,uint32_t tick){
    //show content
    if(this->getB(3)==1){
        // printf("t;");
        // printf("%x%x;\n",this->get4B(4),this->get4B(8));
        for(int i=4;;i++){
            char ch=this->getB(i);
            this->putB(i,0);
            if(ch=='\0'){
                break;
            }
            printf("%c",ch);
        }
        this->putB(3,0);
    }
    return 0;
}
