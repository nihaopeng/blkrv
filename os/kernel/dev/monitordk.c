#include "drivers.h"

int open_monitor_k(){
    *(int*)MONITOR_ADDR=1;
    printk("monitor opened\n");
}

int close_monitor_k(){
    *(int*)MONITOR_ADDR=0;
    printk("monitor closed\n");
}