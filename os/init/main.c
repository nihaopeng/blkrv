// #include "std.h"
#include "console.h"

int main(){
    init_std();
    print("hell%c,%c%c%c%c%c\n",'o','w','o','r','l','d');
    print("%s\n","hello,world2");
    
    print("rrrrrr      vv          vv\n");
    print("rr    rr     vv        vv\n");
    print("rr    rr      vv      vv\n");
    print("rrrrrr         vv    vv\n");
    print("rrrr            vv  vv\n");
    print("rr  rr           vvvv\n");
    print("rr    rr          vv\n\n");
    print("still in building...\n");
    int d_test=-21474;
    print("d fmt test:%d\n",d_test);
    init_console();
    char ch;
    char test[20];
    int a=0;
    print("input test:");
    // input("%d",&a);
    // print("%d\n",a);
    input("%s%d",&test,&a);
    print("%s,%d\n",test,a);
    shutdown();
}
