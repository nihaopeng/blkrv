#include "std.h"

int main(int argc,char* argv[]){
    print("argc:%d,argv_addr:%d\n",argc,argv);
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
    int ifkbhit=0;
    char ch;
    while(1){
        kbhit(&ifkbhit);
        if(ifkbhit){
            vgetch(&ch);
            if(ch=='q')
                break;
        }
    }
    print("##kbhit pass\n");
    uint32_t inode_id=0;
    int status=0;
    create("/tmp/user_program_test.txt",'f',&inode_id,&status);
    print("##create pass\n");
    open("/tmp/user_program_test.txt",&inode_id,&status);
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
    open("/tmp/test.bin",&inode_id,&status);
    while(1){
        recv(&sock,buf1,1024,&status);
        if(status==-1){
            break;
        }
        print("\n%s\n",buf1);
    }
    print("##send and recv pass\n");
    
    point p1={380,280};
    color c={0,255,0};
    draw_label(&p1,"hello, this is user, input 'q' to quit:",&c,180);
    flush();
    while(1){
        kbhit(&ifkbhit);
        if(ifkbhit){
            vgetch(&ch);
            if(ch=='q')
                break;  
        }
    }

    print("graphic tests pass\n");

    print("##all tests pass!\n");
}