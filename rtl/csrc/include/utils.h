#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <cstdlib>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

class utils
{
private:
    /* data */
public:
    static uint32_t kbhit(void);
    static void restore_tty(void);
};

#endif // !_UTILS_H_
