// BLKRV clear — 发送 ANSI 清屏序列, 由宿主侧 terminal 模拟器处理
#include "blkrv.h"

int main(void) {
    write(1, "CLRMARK", 7);         // 调试: 验证 clear 进程是否执行了自己的 main
    write(1, "\033[2J\033[H", 7);
    return 0;
}
