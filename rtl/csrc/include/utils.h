#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

class utils
{
private:
    /* data */
public:
    static int kbhit(void);
};

#endif // !_UTILS_H_
