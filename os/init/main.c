// #include "std.h"
#include "file.h"
#include "proc.h"

int main(){
    regist_sysmethod();
    init_input();
    init_out();
    init_net();
    // init_nvmm();
    init_fs();
    // init_vmm();
    //init_vmm启用内核页表,启用内核页表后，所有的kernel系统调用将不被允许
    //init_ps()应该放在init_vmm之后以便于取得satp的值从而初始化shell进程。
    // init_ps();
    //init_console是shell进程，运行在线性内核空间，pid=1。

    // printk("BBBB     LL      K   K\n");
    // printk("B   B    LL      KK K\n");
    // printk("BBBB     LL      KKK\n");
    // printk("B   B    LL      KK K\n");
    // printk("BBBB     LLLLLLL K   K\n");
    

    /*
        test graphics
    */
    // point p1={300,280};
    // color c={255,0,0};
    // draw_labelk(&p1,"BLKRv",&c,80);
    // flushk();
    // while(1);
    // point p1={200,400};
    // point p2={300,300};
    // point p3={400,400};
    // color c={255,0,0};
    // while(1){
    //     p1.x+=1;p2.x+=1;p3.x+=1;
    //     draw_trianglek(&p1,&p2,&p3,&c);
    //     flushk();
    // }
    // while(1);
    inode ino;
    uint32_t tmp=0;
    uint32_t inode_id=createk("/tmp/test.jpg",FILE_TYPE,&tmp);
    // open_monitor_k();
    finfo_k(inode_id,&ino);
    // close_monitor_k();
    printk("inode_id:%d,file size:%d\n",inode_id,ino.size);
    // below is a simple wget
    if(ino.size!=136813){
        socket sock={0,"127.0.0.1",8080};
        char message[]="jpg";
        char buf[1024];
        memset_s(buf,0,1024);
        sendk(&sock,message,str_len(message));
        uint32_t fp=0;
        for(int i=0;;i++){
            uint32_t data_len=recvk(&sock,buf,1024);
            if(data_len==0){
                break;
            }
            writek(inode_id,buf,fp,data_len);
            fp+=data_len;
            printk("file pointer:%d\n",fp);
        }
    }
    finfo_k(inode_id,&ino);
    draw_jpgk(inode_id,ino.size,0,0);
    flushk();
    while(1);

    /*
        get exec file from server and exec it.
    */
    
    // inode ino;
    // uint32_t tmp=0;
    // uint32_t inode_id=createk("/bin/hello.elf",FILE_TYPE,&tmp);
    // // open_monitor_k();
    // finfo_k(inode_id,&ino);
    // // close_monitor_k();
    // printk("inode_id:%d,file size:%d\n",inode_id,ino.size);
    // // below is a simple wget
    // if(ino.size!=35780){
    //     socket sock={0,"127.0.0.1",8080};
    //     char message[]="testfile";
    //     char buf[1024];
    //     memset_s(buf,0,1024);
    //     sendk(&sock,message,str_len(message));
    //     uint32_t fp=0;
    //     for(int i=0;;i++){
    //         uint32_t data_len=recvk(&sock,buf,1024);
    //         if(data_len==0){
    //             break;
    //         }
    //         writek(inode_id,buf,fp,data_len);
    //         fp+=data_len;
    //         printk("file pointer:%d\n",fp);
    //     }
    // }
    // // finfo_k(inode_id,&ino);
    // // printk("inode_id:%d,inode:%x,file size:%d\n",inode_id,&ino,ino.size);
    // // deletek(inode_id);//删除文件
    // // finfo_k(inode_id,&ino);
    // // printk("inode_id:%d,inode:%x,file size:%d\n",inode_id,&ino,ino.size);
    // // printk("after delete id:%d\n",openk("/bin/hello.elf"));
    // //exec file
    // // printk("fid:%d\nexec file...\b\n",inode_id);
    // char para1[]="./user_program.bin";
    // char para2[]="1234";
    // char para3[]="578";
    // char para4[]="wuhu";
    // char para5[]="enheng";
    // char para6[]="he";
    // char* para[]={para1,para2,para3,para4,para5,para6};
    // // printk("%d,%d,%d,%d,%d,%d",para,&para[1],&para[2],&para[3],&para[4],&para[5]);
    // execk(inode_id,-1,para,6);

    /*
        test input
    */
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
    
    /*
        test monitor.
    */
    // open_monitor_k();
    // char s[1024];
    // for(int i=0;i<1024;i++){
    //     s[i]=0;
    // }
    // inputk("%s",s);
    // printk("input: %s\n",s);
    // for(int i=0;i<1024;i++){
    //     s[i]=0;
    // }
    // close_monitor_k();

    shutdown();
}
