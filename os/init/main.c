#include "std.h"
#include "file.h"
#include "proc.h"

int get_file_from_server(char* remote_file_name,char* local_file_name,char* ip,uint32_t port){
    inode ino;
    uint32_t tmp=0;
    printk("get file:%s from server to file:%s\n",local_file_name,remote_file_name);
    uint32_t inode_id=openk(local_file_name);
    if(inode_id==-1){
        printk("file not exist, create it\n");
        inode_id=createk(local_file_name,FILE_TYPE,&tmp);
    }
    // open_monitor_k();
    finfo_k(inode_id,&ino);
    // close_monitor_k();
    printk("inode_id:%d,file size:%d\n",inode_id,ino.size);
    // below is a simple wget
    socket sock;
    sock.origin_port=0;
    // printk("ip:%s,port:%d\n",ip,port);
    memset_s(sock.ip,0,16);
    str_cpy(ip,sock.ip);
    sock.target_port=port;
    char* message=remote_file_name;
    char buf[1024];
    memset_s(buf,0,1024);
    int sockfd=connectk(&sock);
    // printk("connect to server:%s:%d\n",sock.ip,sock.target_port);
    sendk(sockfd,message,str_len(message));
    
    recvk(sockfd,buf,4);//获得文件大小
    uint32_t recv_data_len=*(uint32_t*)buf;
    // if(recv_data_len==ino.size){
    //     printk("%d:%d file size is same, no need to download\n",recv_data_len,ino.size);
    //     return inode_id;
    // }
    printk("%d:%d file size is diff, need to download\n",recv_data_len,ino.size);
    uint32_t fp=0;
    while(fp<recv_data_len){
        uint32_t data_len=recvk(sockfd,buf,1024);
        if(data_len==0){
            break;
        }
        writek(inode_id,buf,fp,data_len);
        fp+=data_len;
        printk("file pointer:%d\r",fp);
    }
    closek(sockfd);
    return inode_id;
}

int gl(char* str){
    uint32_t p=0;
    while(1){
        // print("getting ch\n");
        char ch=vgetchk();
        if(ch==10){
            // printk("enter]%c",ch);
            break;
        }else if(ch!=0){
            // printk("%d ",ch);
            if(ch==127){
                p-=1;
                str[p]=0;
            }else{
               str[p++]=ch;
            }
        }
    }
    return p;
}

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

    printk("BBBB     LL      K   K\n");
    printk("B   B    LL      KK K\n");
    printk("BBBB     LL      KKK\n");
    printk("B   B    LL      KK K\n");
    printk("BBBB     LLLLLLL K   K\n");
    
    // inode ino;
    // char ppt1[]="/tmp/1.png";
    // char ppt2[]="/tmp/2.png";
    // char ppt3[]="/tmp/3.png";
    // char ppt4[]="/tmp/4.png";
    // char ppt5[]="/tmp/5.png";
    // char ppt6[]="/tmp/6.png";
    // char* ppts[6]={ppt1,ppt2,ppt3,ppt4,ppt5,ppt6};
    // get_file_from_server("./ppt_pngs/1.png",ppts[0],"127.0.0.1\0",8080);
    // get_file_from_server("./ppt_pngs/2.png",ppts[1],"127.0.0.1\0",8080);
    // get_file_from_server("./ppt_pngs/3.png",ppts[2],"127.0.0.1\0",8080);
    // get_file_from_server("./ppt_pngs/4.png",ppts[3],"127.0.0.1\0",8080);
    // get_file_from_server("./ppt_pngs/5.png",ppts[4],"127.0.0.1\0",8080);
    // get_file_from_server("./ppt_pngs/6.png",ppts[5],"127.0.0.1\0",8080);
    // uint32_t inode_id=0;
    // int cur_page=0;
    // inode_id=openk(ppts[cur_page]);
    // finfo_k(inode_id,&ino);
    // open_monitor_k();
    // draw_pngk(inode_id,ino.size,0,0);
    // close_monitor_k();
    // flushk();
    //  while(1){
    //      if(kbhitk()){
    //          char ch=vgetchk();
    //          if(ch=='j'){
    //              cur_page=cur_page-1>=0?cur_page-1:0;
    //              printk("cur_page:%d\n",cur_page);
    //              inode_id=openk(ppts[cur_page]);
    //              finfo_k(inode_id,&ino);
    //              draw_pngk(inode_id,ino.size,0,0);
    //              flushk();
    //          }
    //          else if(ch=='q'){
    //              break;
    //          }
    //          else if(ch=='k'){
    //              cur_page=cur_page+1<=5?cur_page+1:5;
    //              inode_id=openk(ppts[cur_page]);
    //              finfo_k(inode_id,&ino);
    //              draw_pngk(inode_id,ino.size,0,0);
    //              flushk();
    //          }
    //      }
    //  }

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

    /*
        get exec file from server and exec it.
    */

    // inode ino;
    // uint32_t tmp=0;
    // uint32_t inode_id=createk("/bin/snake_game",FILE_TYPE,&tmp);
    // // open_monitor_k();
    // finfo_k(inode_id,&ino);
    // // close_monitor_k();
    // printk("inode_id:%d,file size:%d\n",inode_id,ino.size);
    // // below is a simple wget
    // get_file_from_server("./snake_game/snake","/bin/snake_game","127.0.0.1",8080);
    // char para1[]="./snake_game";
    // char* para[]={para1};
    // execk(inode_id,-1,para,1);

    inode ino;
    uint32_t tmp=0;
    uint32_t inode_id=createk("/bin/test_cli",FILE_TYPE,&tmp);
    // open_monitor_k();
    finfo_k(inode_id,&ino);
    // close_monitor_k();
    printk("inode_id:%d,file size:%d\n",inode_id,ino.size);
    // below is a simple wget
    get_file_from_server("./test_cli/test_cli","/bin/test_cli","127.0.0.1",8080);
    char para1[]="./test_cli";
    char* para[]={para1};
    execk(inode_id,-1,para,1);

    /*
        test input
    */
    // char s[20];
    // printk("getline:");
    // gl(s);
    // printk("gl:%s\n",s);
    // int ifkbhit=0;
    // char ch;
    // char iskbhit=0;
    // printk("press q to exit\n");
    // while(1){
    //     iskbhit = kbhitk();
    //     if(iskbhit){
    //         ch = vgetchk();
    //         if(ch=='q')
    //             break;
    //         printk("\033[1;40;31mchar:%c,int:%d\033[0m",ch,ch);
    //     }
    // }
    
    /*
        test monitor.
    */
    // open_monitor_k();
    // finfo_k(inode_id,&ino);
    // close_monitor_k();

    shutdown();
}
