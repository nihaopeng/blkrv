// #include "std.h"
#include "console.h"

int main(){
    init_std();
    init_fs();
    load_blocks();
    init_console();
    int inode_id=-1,status=-2;
    open("/",&inode_id,&status);
    if(status==-1){
        print("root directory is not exist\n");
        uint32_t root_id=0;
        create("/",'d',&root_id);
    }
    open("/",&inode_id,&status);
    print("root:%d\n",inode_id);
    int home_id=0;
    create("/home",'d',&home_id);
    open("/home",&inode_id,&status);
    print("home_id:%d,status:%d\n",inode_id,status);
    shutdown();
}
