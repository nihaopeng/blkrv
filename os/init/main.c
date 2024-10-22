// #include "std.h"
#include "console.h"

int main(){
    init_std();
    init_fs();
    print("rrrrrr      vv         vv\n");
    print("rr    rr     vv       vv\n");
    print("rr    rr      vv     vv\n");
    print("rrrrrr         vv   vv\n");
    print("rrrr            vv vv\n");
    print("rr  rr           vvv\n");
    print("rr    rr          v\n\n");
    init_console();
    int inode_id=-1,status=0;
    open("/",&inode_id,&status);
    print("inode:%d",inode_id);
    shutdown();
}
