// #include "std.h"
#include "console.h"
#include "file.h"
#include "proc.h"

int main(int argc,char* argv[]){
    set_stdout(-1,0);
    init_std();
    init_fs();
    load_blocks();
    init_console();
    root();

    // socket sock={0,"127.0.0.1",8080};
    // char message[]="testfile";
    // char buf[512];
    // memset_s(buf,0,512);
    // send(sock,message,str_len(message));
    // int status=0;
    // uint32_t inode_id=0;
    // open("/tmp/test.bin",&inode_id,&status);
    // inode* ino;
    // get_inode_by_id(inode_id,&ino);
    // while(1){
    //     recv(sock,buf,32,&status);
    //     if(status==-1){
    //         break;
    //     }
    //     write(inode_id,buf,ino->size,32);
    // }
    uint32_t inode_id;
    uint32_t pid;
    int status=0;
    inode* ino;
    open("/tmp/test.bin",&inode_id,&status);
    print("fid:%d\nexec file...\n",inode_id);
    exec(inode_id,0,-1,0,&pid,&status,NULL);
    shutdown();
}
