// BLKRV rm — 删除文件/目录 (按 inode id)
#include "blkrv.h"

int main(int argc, char* argv[]) {
    if (argc < 2) { print("Usage: rm <file>\n"); return 1; }
    int fd = open(argv[1]);
    if (fd < 0) { print("rm: not found\n"); return 1; }
    if (delete(fd) < 0) { print("rm: failed (dir not empty?)\n"); return 1; }
    return 0;
}
