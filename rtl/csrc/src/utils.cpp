#include "utils.h"

uint32_t utils::kbhit(void)
{
    struct termios oldt, newt;
    uint32_t ch,ch1,ch2;
    int result=0;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    if(ch != EOF){
        result=result|ch;
        ch1 = getchar();
        if(ch1 != EOF){
            result=result|(ch1<<8);
            ch2 = getchar();
            if(ch2 != EOF){
                result=result|(ch2<<16);
            }
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    return result;
}