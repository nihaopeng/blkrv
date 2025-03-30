// #include "std.h"
#include "console.h"
#include "file.h"
#include "proc.h"

int main(int argc,char* argv[]){
    regist_sysmethod();
    init_input();
    init_out();
    init_net();
    init_ps();
    init_nvmm();
    init_fs();
    init_vmm();
    //此时已启用内核页表
    print("finish init_vmm\n");
    init_console();

    //test gpu
    // point p1={300,280};
    // color c={255,0,0};
    // draw_labelk(&p1,"BLKRv",&c,80);
    // flushk();
    // while(1);

    // int status=0;
    // uint32_t inode_id=0;

    // // below is a simple wget
    // socket sock={0,"127.0.0.1",8080};
    // char message[]="testfile";
    // char buf[1024];
    // memset_s(buf,0,1024);
    // sendk(&sock,message,str_len(message));
    // openk("/tmp/test.bin",&inode_id,&status);
    // inode* ino;
    // get_inode_by_id(inode_id,&ino);
    // printk("\n");
    // for(int i=0;;i++){
    //     recvk(&sock,buf,1024,&status);
    //     if(status==-1){
    //         break;
    //     }
    //     writek(inode_id,buf,mul(i,1024),1024);
    //     printk("size:%d\n",ino->size);
    // }

    // //exec your program
    // uint32_t pid;
    // openk("/tmp/test.bin",&inode_id,&status);
    // printk("fid:%d\nexec file...\b\n",inode_id);
    // char para1[]="./user_program.bin";
    // char para2[]="1234";
    // char para3[]="578";
    // char para4[]="wuhu";
    // char para5[]="enheng";
    // char para6[]="he";
    // char* para[]={para1,para2,para3,para4,para5,para6};
    // printk("%d,%d,%d,%d,%d,%d",para,&para[1],&para[2],&para[3],&para[4],&para[5]);
    // exec(inode_id,0,-1,0,&pid,&status,para,7);

    // char s[20];
    // printk("getline:");
    // getline(s);
    // printk("%s",s);
    // int ifkbhit=0;
    // char ch;
    // while(1){
    //     kbhit(&ifkbhit);
    //     if(ifkbhit){
    //         vgetch(&ch);
    //         if(ch=='q')
    //             break;
    //         // print("\033[1;40;31mchar:%c,int:%d\033[0m",ch,ch);
    //     }
    // }
    
    // open_monitor();
    // char s[1024];
    // for(int i=0;i<1024;i++){
    //     s[i]=0;
    // }
    // print("test here");
    // while(1){
    //     int p=getline(s);
    //     for(int i=0;i<p;i++){
    //         print("%d ",s[i]);
    //         s[i]=0;
    //     }
    //     print("\n");
    // }
    // open_monitor();
    // close_monitor();
    // close_monitor();

    shutdown();
}
