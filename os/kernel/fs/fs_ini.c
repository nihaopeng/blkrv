#include "file.h"

int root(){
    uint32_t inode_id;
    int status;
    create("/",'d',&inode_id,&status);
    create("/include",'d',&inode_id,&status);
    
}