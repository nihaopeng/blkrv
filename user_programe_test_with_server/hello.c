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
    print("##vgetch pass\ninput 'q' to test:");
    while(1){
        if(kbhit()){
            if(vgetch()=='q')
                break;
        }
    }
    print("##kbhit pass\n");
    uint32_t inode_id=0;
    create("/tmp/user_program_test.txt",'f',&inode_id);
    print("##create pass\n");
    inode_id=open("/tmp/user_program_test.txt");
    print("##open pass\n");
    char buf[]="hello world";
    write(inode_id,buf,0,str_len(buf));
    print("##write pass\n");
    char read_buf[20];
    read(inode_id,read_buf,0,20);
    print("%s\n",read_buf);
    print("##read pass\n");

    socket sock={0,"127.0.0.1",8080};
    char message[]="send test";
    char buf1[1024];
    send(&sock,message,str_len(message));
    // inode_id=open("/tmp/test.bin");
    while(1){
        if(recv(&sock,buf1,1024)==0){
            break;
        }
        print("\n%s\n",buf1);
    }
    print("##send and recv pass\n");

    //test vmm
    char* tmp=(char*)malloc(30);
    tmp="this is blkrv malloc test";
    print("tmp:%s\n",tmp);
    char* tmp1=(char*)malloc(30);
    tmp1="this is blkrv malloc test1";
    print("tmp1:%s\n",tmp1);
    char* tmp2=(char*)malloc(30);
    tmp2="this is blkrv malloc test2";
    print("tmp2:%s\nfree tmp1\n",tmp2);
    free(tmp1);

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
    //     if(ifkbhit()){
    //         if(vgetch()=='q')
    //             break;
    //     }
    // }

    // print("graphic tests pass\n");

    // print("##all tests pass!\n");
    return 0;
}