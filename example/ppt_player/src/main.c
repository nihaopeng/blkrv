#include "std.h" 
#include "str.h"
// #include "math.h"

int memset_s(char* addr,char ch,uint32_t count){
    for(int i=0;i<count;i++){
        *(addr) = ch;
        addr+=1;
    }
    return 0;
}

int get_file_from_server(char* remote_file_name,char* local_file_name,char* ip,uint32_t port){
    inode ino;
    uint32_t tmp=0;
    print("get file:%s from server to file:%s\n",remote_file_name,local_file_name);
    uint32_t inode_id=open(local_file_name);
    if(inode_id==-1){
        print("file not exist, create it\n");
        inode_id=create(local_file_name,FILE_TYPE,&tmp);
    }
    // open_monitor_k();
    finfo(inode_id,&ino);
    // close_monitor_k();
    print("inode_id:%d,file size:%d\n",inode_id,ino.size);
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
    int sockfd=connect(&sock);
    // printk("connect to server:%s:%d\n",sock.ip,sock.target_port);
    send(sockfd,message,str_len(message));
    
    recv(sockfd,buf,4);//获得文件大小
    uint32_t recv_data_len=*(uint32_t*)buf;
    if(recv_data_len==ino.size){
        print("%d:%d file size is same, no need to download\n",recv_data_len,ino.size);
        return inode_id;
    }
    print("%d:%d file size is diff, need to download\n",recv_data_len,ino.size);
    uint32_t fp=0;
    while(fp<recv_data_len){   
        uint32_t data_len=recv(sockfd,buf,1024);
        if(data_len==0){
            break;
        }
        write(inode_id,buf,data_len);
        fp+=data_len;
        print("file pointer:%d\r",fp);
    }
    close(sockfd);
    return inode_id;
}

int main(int argc, char* argv[]) {
    int ppt_num=36;
    char** remote_ppts=(char**)malloc(sizeof(char*)*(ppt_num+1));
    char** local_ppts=(char**)malloc(sizeof(char*)*(ppt_num+1));
    char remote_dir[]="./ppt_pngs/";
    char local_dir[]="/tmp/";
    for(int i=1;i<=ppt_num;i++){
        remote_ppts[i]=(char*)malloc(sizeof(char)*32);
        str_cpy("./ppt_pngs/xx.png",remote_ppts[i]);
        remote_ppts[i][str_len(remote_dir)]='0'+i/10;
        remote_ppts[i][str_len(remote_dir)+1]='0'+i%10;
        print("remote_ppts[%d]:%s\n",i,remote_ppts[i]);
        local_ppts[i]=(char*)malloc(sizeof(char)*32);
        str_cpy("/tmp/xx.png",local_ppts[i]);
        local_ppts[i][str_len(local_dir)]='0'+i/10;
        local_ppts[i][str_len(local_dir)+1]='0'+i%10;
        print("local_ppts[%d]:%s\n",i,local_ppts[i]);
    }
    for(int i=1;i<=ppt_num;i++){
        get_file_from_server(remote_ppts[i],local_ppts[i],"127.0.0.1",8080);
    }
    inode ino;
    uint32_t inode_id=0;
    int cur_page=1;
    inode_id=open(local_ppts[cur_page]);
    finfo(inode_id,&ino);
    draw_png(inode_id,ino.size,0,0);
    flush();
     while(1){
         char ch;
         if(read(STDIN_FILENO,&ch,1)>0){
             if(ch=='j'){
                 cur_page=cur_page-1>=1?cur_page-1:0;
                 print("cur_page:%d\n",cur_page);
                 inode_id=open(local_ppts[cur_page]);
                 finfo(inode_id,&ino);
                 draw_png(inode_id,ino.size,0,0);
                 flush();
             }
             else if(ch=='q'){
                 break;
             }
             else if(ch=='k'){
                 cur_page=cur_page+1<=ppt_num?cur_page+1:5;
                 inode_id=open(local_ppts[cur_page]);
                 finfo(inode_id,&ino);
                 draw_png(inode_id,ino.size,0,0);
                 flush();
             }
         }
     }
}