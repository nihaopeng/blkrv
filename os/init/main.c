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

    // below is a simple wget
    socket sock={0,"127.0.0.1",8080};
    char message[]="testfile";
    char buf[1024];
    memset_s(buf,0,1024);
    sendk(&sock,message,str_len(message));
    openk("/tmp/test.bin",&inode_id,&status);
    inode* ino;
    get_inode_by_id(inode_id,&ino);
    printk("\n");
    for(int i=0;;i++){
        recvk(&sock,buf,1024,&status);
        if(status==-1){
            break;
        }
        writek(inode_id,buf,mul(i,1024),1024);
        printk("size:%d\n",ino->size);
    }

    //exec your program
    uint32_t pid;
    openk("/tmp/test.bin",&inode_id,&status);
    printk("fid:%d\nexec file...\b\n",inode_id);
    char para1[]="./user_program.bin";
    char para2[]="1234";
    char para3[]="578";
    char para4[]="wuhu";
    char para5[]="enheng";
    char para6[]="he";
    char* para[]={para1,para2,para3,para4,para5,para6};
    printk("%d,%d,%d,%d,%d,%d",para,&para[1],&para[2],&para[3],&para[4],&para[5]);
    exec(inode_id,0,-1,0,&pid,&status,para,7);

    shutdown();
}
