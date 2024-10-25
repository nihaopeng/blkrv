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
    create("/home1/test",'d',&inode_id,&status);
    if(status==-2){
        print("file has been exists\n");
    }else if(status==-1){
        print("path not exists\n");
    }
    open("/home1/test",&inode_id,&status);
    get_inode_by_id(inode_id,&ino);
    if(status!=-1)
        print("home_inode_id:%d;start_b:%d\n",inode_id,ino->start_block);
    else
        print("file not exists\n");
    shutdown();
}
