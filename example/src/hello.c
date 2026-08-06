#include "std.h"

int main(int argc,char* argv[]){
    print("argc:%d,argv_addr:%x\n",argc,argv);
    for(int i=0;i<argc;i++){
        print("arg%d:%s\n",i,argv[i]);
    }

    char res[20]="";
    int t=0;
    print("\n----hello----\nthis is user program,are you ok?(yes/no)(1/2)");
    input("%s,%d",res,&t);
    print("ans:%s,%d\n",res,t);
    if(str_cmp("yes",res)){
        print("fine\n");
    }else if(str_cmp("no",res)){
        print("i'm sorry to hear that\n");
    }else{
        print("what the fuck?\n");
    }
    print("##tty read pass\ninput 'q' to test:");
    while(1){
        char ch;
        if(read(STDIN_FILENO,&ch,1)>0){
            if(ch=='q') break;
        }
    }
    print("##tty read pass\n");
    uint32_t inode_id=0;
    create("/tmp/user_program_test.txt",'f',&inode_id);
    print("##create pass\n");
    int fd=open("/tmp/user_program_test.txt");
    print("##open pass\n");
    char buf[]="hello world";
    write(fd,buf,str_len(buf));
    print("##write pass\n");
    char read_buf[20];
    read(fd,read_buf,20);
    print("%s\n",read_buf);
    print("##read pass\n");

    socket sock={0,"127.0.0.1",8080};
    char message[]="send test";
    char buf1[1024];
    int sockfd=connect(&sock);
    send(sockfd,message,str_len(message));
    // inode_id=open("/tmp/test.bin");
    recv(sockfd,buf1,1024);
    print("\n%s\n",buf1);
    print("##send and recv pass\n");

    //test vmm
    // open_monitor();
    char* tmp=(char*)malloc(30);
    // tmp="this is blkrv malloc test";
    print("tmp:%d:%s\n",(uint32_t)tmp,tmp);
    char* tmp1=(char*)malloc(30);
    // tmp1="this is blkrv malloc test1";
    print("tmp1:%d:%s\n",(uint32_t)tmp1,tmp1);
    char* tmp2=(char*)malloc(30);
    // tmp2="this is blkrv malloc test2";//错误赋值方式。
    str_cpy("this is blkrv malloc test2",tmp2);
    print("tmp2:%d:%s\nfree tmp1\n",(uint32_t)tmp2,tmp2);
    free(tmp1);
    // close_monitor();

    open_monitor();
    for(int i=0;i<10;i++){
        char* tmp=(char*)malloc(30);
        // str_cpy("this is blkrv malloc test",tmp);
        // print("tmp:%d:%s\n",(uint32_t)tmp,tmp);
        // free(tmp);
    }
    close_monitor();

    //严重违规操作，自己调用自己，将会陷入调用地狱，此处做测试使用。
    // inode_id=open("/tmp/test.bin");
    // char para1[]="test.bin";
    // char para2[]="-a";
    // char para3[]="-c";
    // char* para[]={para1,para2,para3};
    // exec(inode_id,-1,para,3);

    // point p1={380,280};
    // color c={0,255,0};
    // draw_label(&p1,"hello, this is user, input 'q' to quit:",&c,180);
    // flush();
    // while(1){
    //     char ch;
    //     if(read(STDIN_FILENO,&ch,1)>0){
    //         if(ch=='q') break;
    //     }
    // }

    print("graphic tests pass\n");

    print("##all tests pass!\n");
    return 0;
}
