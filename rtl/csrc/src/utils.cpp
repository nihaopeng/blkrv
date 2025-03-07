#include "utils.h"

uint32_t utils::kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF)
    {
        //上下左右组合键,[x,3,2,1]
        if(ch == 27){
            char ch1 = getchar();
            char ch2 = getchar();
            return (uint32_t)((uint32_t)ch|(uint32_t)ch1<<8|(uint32_t)ch2<<16);
        }
        //[x,x,x,1]
        return (uint32_t)ch;
    }
    return 0;
}