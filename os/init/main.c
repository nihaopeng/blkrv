// #include "std.h"
#include "console.h"
#include "file.h"

int main(){
    init_std();
    init_fs();
    load_blocks();
    init_console();
    int inode_id=-1,status=-2;
    inode* ino;
    open("/",&inode_id,&status);
    // print("root_id:%d;start_b:%d;size:%d\n",inode_id,ino->start_block,ino->size);
    if(status==-1){
        print("root directory is not exist\n");
        create("/",'d',&inode_id,&status);
        get_inode_by_id(inode_id,&ino);
        print("inode_id:%d;start_b:%d\n",inode_id,ino->start_block);
    }
    create("/home",'d',&inode_id,&status);
    create("/home/test",'d',&inode_id,&status);
    create("/home/test/test",'f',&inode_id,&status);
    create("/home/test1",'f',&inode_id,&status);
    open("/home/test/test",&inode_id,&status);
    char t[4500];
    memset_s(t,'t',4500);
    char t1[4500];
    if(!status){
        get_inode_by_id(inode_id,&ino);
        write(inode_id,t,0,45);
        read(inode_id,t1,0,4500);
        print("read_len:%d\n",str_len(t1));
    }else{
        print("can not open file\n");
    }
    delete_inode(inode_id);
    shutdown();
}
