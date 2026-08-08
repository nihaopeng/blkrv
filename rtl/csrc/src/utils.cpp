#include "utils.h"

// 终端 raw 模式 + 非阻塞只配置一次, 之后每拍只需一次 read() 系统调用。
// 旧实现每次 kbhit 都做 tcgetattr/tcsetattr/fcntl/getchar 共 6 次系统调用,
// 而 keyboard::process 每 tick 都会调用, 是编辑器卡顿的主要瓶颈。
static bool g_raw_setup = false;
static struct termios g_old_tio;
static int g_old_flags = -1;

static void raw_setup_once(void){
    if(g_raw_setup) return;
    tcgetattr(STDIN_FILENO, &g_old_tio);
    struct termios newt = g_old_tio;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    g_old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, g_old_flags | O_NONBLOCK);
    g_raw_setup = true;
}

void utils::restore_tty(void){
    if(!g_raw_setup) return;
    tcsetattr(STDIN_FILENO, TCSANOW, &g_old_tio);
    if(g_old_flags >= 0) fcntl(STDIN_FILENO, F_SETFL, g_old_flags);
    g_raw_setup = false;
}

uint32_t utils::kbhit(void)
{
    raw_setup_once();
    unsigned char buf[3];
    ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
    if(n <= 0) return 0;
    uint32_t result = 0;
    for(ssize_t i=0;i<n;i++) result |= ((uint32_t)buf[i]) << (8*i);
    return result;
}
