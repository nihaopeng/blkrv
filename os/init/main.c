// #include "std.h"
#include "console.h"
#include "file.h"

int main(){
    init_std();
    init_fs();
    load_blocks();
    init_console();
    root();
    // send();
    // recv();
    
    shutdown();
}
