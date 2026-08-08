// BLKRV cat — 输出文件内容
#include "blkrv.h"

int main(int argc, char* argv[]) {
    if (argc < 2) { print("Usage: cat <file>\n"); return 1; }

    int fd = open(argv[1]);
    if (fd < 0) { print("cat: open failed\n"); return 1; }

    char buf[128];
    int n;
    while ((n = read(fd, buf, 128)) > 0) {
        write(1, buf, n);
    }
    close(fd);
    return 0;
}
