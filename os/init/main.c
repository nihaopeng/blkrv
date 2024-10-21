// #include "std.h"
#include "console.h"

int main(){
    init_std();
    print("hell%c,%c%c%c%c%c\n",'o','w','o','r','l','d');
    print("%s\n","hello,world2"); 
    print("rrrrrr      vv         vv\n");
    print("rr    rr     vv       vv\n");
    print("rr    rr      vv     vv\n");
    print("rrrrrr         vv   vv\n");
    print("rrrr            vv vv\n");
    print("rr  rr           vvv\n");
    print("rr    rr          v\n\n");
    print("still in building...\n");
    init_console();
    int a=0;
    print("input:");
    input("%d",&a);
    print("%d",a);
    while(1){
        a=0;
        kbhit(&a);
        if(a){
            char ch;vgetch(&ch);
            print("hit:%c\n",ch);
            if(ch=='q')break;
        }
    }
    shutdown();
}
