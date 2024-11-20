#include "std.h"

int main(){
    char res[20];
    print("\n----hello----\nthis is user program,are you ok?(y/n)");
    input("%s",res);
    if(str_cmp("y",res)){
        print("fine\n");
    }else{
        print("i'm sorry to hear that\n");
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
    print("##all tests pass!\n");
}