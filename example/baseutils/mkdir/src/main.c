// BLKRV mkdir — 创建目录
#include "blkrv.h"

int main(int argc, char* argv[]) {
    if (argc < 2) { print("Usage: mkdir <dir>\n"); return 1; }
    unsigned id;
    if (create(argv[1], 'd', &id) < 0) { print("mkdir: failed\n"); return 1; }
    return 0;
}
