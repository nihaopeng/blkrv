// BLKRV touch — 创建空文件
#include "blkrv.h"

int main(int argc, char* argv[]) {
    if (argc < 2) { print("Usage: touch <file>\n"); return 1; }
    unsigned id;
    if (create(argv[1], 'f', &id) < 0) { print("touch: failed\n"); return 1; }
    return 0;
}
