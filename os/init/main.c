// #include "std.h"
#include "console.h"
#include "file.h"
#include "proc.h"

int main(int argc,char* argv[]){
    regist_sysmethod();
    init_input();
    init_out();
    init_net();
    init_mm();
    init_ps();
    init_fs();
    init_console();

    int status=0;
    uint32_t inode_id=0;

    //below is a simple wget
    socket sock={0,"127.0.0.1",8080};
    char message[]="testfile";
    char buf[1024];
    memset_s(buf,0,1024);
    send_i(&sock,message,str_len(message));
    open_i("/tmp/test.bin",&inode_id,&status);
    inode* ino;
    get_inode_by_id(inode_id,&ino);
    printk("\n");
    while(1){
        recv_i(&sock,buf,1024,&status);
        if(status==-1){
            break;
        }
        printk("%d\r",ino->size);
        write_i(inode_id,buf,ino->size,1024);
    }

    //exec your program
    uint32_t pid;
    open_i("/tmp/test.bin",&inode_id,&status);
    printk("fid:%d\nexec file...\n",inode_id);
    exec(inode_id,0,-1,0,&pid,&status,NULL);

    shutdown();
}
