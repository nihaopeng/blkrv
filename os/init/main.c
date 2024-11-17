// #include "std.h"
#include "console.h"
#include "file.h"
#include "proc.h"

int main(int argc,char* argv[]){
    set_stdout(-1,0);
    init_std();
    init_fs();
    init_proc();
    load_blocks();
    init_console();
    root();

    socket sock={0,"127.0.0.1",8080};
    char message[]="testfile";
    char buf[1024];
    memset_s(buf,0,1024);
    send(sock,message,str_len(message));
    int status=0;
    uint32_t inode_id=0;
    open("/tmp/test.bin",&inode_id,&status);
    inode* ino;
    get_inode_by_id(inode_id,&ino);
    while(1){
        recv(sock,buf,1024,&status);
        if(status==-1){
            break;
        }
        write(inode_id,buf,ino->size,1024);
    }

    uint32_t pid;
    open("/tmp/test.bin",&inode_id,&status);
    printk("fid:%d\nexec file...\n",inode_id);
    exec(inode_id,0,-1,0,&pid,&status,NULL);

    shutdown();
}
