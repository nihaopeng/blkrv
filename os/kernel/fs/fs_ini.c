#include "file.h"

int root(){
    uint32_t inode_id;
    int status;
    create_i("/",'d',&inode_id,&status);
    create_i("/include",'d',&inode_id,&status);
    create_i("/tmp",'d',&inode_id,&status);
    create_i("/tmp/test.bin",'f',&inode_id,&status);
}