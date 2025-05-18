// #include "std.h"
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
    if(recv_data_len==ino.size){
        printk("%d:%d file size is same, no need to download\n",recv_data_len,ino.size);
        return inode_id;
    }
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

int main(){
    regist_sysmethod();
    init_input();
    init_out();
    init_net();
    // init_nvmm();
    init_fs();

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
    
    inode ino;
    uint32_t tmp=0;
    uint32_t inode_id=createk("/bin/hello.elf",FILE_TYPE,&tmp);
    // open_monitor_k();
    finfo_k(inode_id,&ino);
    // close_monitor_k();
    printk("inode_id:%d,file size:%d\n",inode_id,ino.size);
    // below is a simple wget
    get_file_from_server("./hello","/bin/hello.elf","127.0.0.1",8080);
    char para1[]="./user_program.bin";
    char para2[]="1234";
    char para3[]="578";
    char para4[]="wuhu";
    char para5[]="enheng";
    char para6[]="he";
    char* para[]={para1,para2,para3,para4,para5,para6};
    // printk("%d,%d,%d,%d,%d,%d",para,&para[1],&para[2],&para[3],&para[4],&para[5]);
    execk(inode_id,-1,para,6);

    shutdown();
}
