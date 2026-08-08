#ifndef _INI_H_
#define _INI_H_

#define uint32_t unsigned int
#define uint16_t unsigned short
#define uint8_t unsigned char 

#define MAX_PRO_NUM 32

//fs
#define MAX_NAME 128
#define MAX_PATH_LEN 256
#define MAX_DEPTH 16
#define DIR_TYPE 'd'
#define FILE_TYPE 'f'
#define BLOCK_SIZE 1024
#define EOF 0xffffffff
#define ROOT_INODE_ID 0

//ram
#define PAGE_SIZE 4096
#define RAM_SIZE 1<<28
#define USER_START 4<<20
#define MAX_PAGE 65536
#define RAM_START 0x00100000

//size is 1
#define KEYBOARD_CACHE_ADDR 0x10100000

// 帧缓存文本终端: 寄存器区 20 字节 + W*H 字符帧数据
#define SCREEN_WIDTH_ADDR  0x10200000  // uint32: 列数 W
#define SCREEN_HEIGHT_ADDR 0x10200004  // uint32: 行数 H
#define SCREEN_FRAME_ROW   0x10200008  // uint32: 光标行 (1-based)
#define SCREEN_FRAME_COL   0x1020000C  // uint32: 光标列 (1-based)
#define SCREEN_CTRL_ADDR   0x10200010  // 1B: ctrl=2 触发渲染
#define SCREEN_CACHE1_ADDR 0x10200014  // 帧数据起始 (W*H 字节)
#define SCREEN_CACHE2_ADDR 0x10210004  // 保留
#define SCREEN_DIRTY_ADDR  0x10210004  // h 字节脏行位图 (内核写, 设备读后清零)

#define GPU_ADDR_CACHE 0x20200000

#define NIC_CTRL_ADDR 0x30200000

// #define FILE_TABLE_ADDR 0x40400000//2MB前2MB为kernel程序存放位置
// #define FILE_DATA_ADDR 0x40500000//3MB
#define INODE_START 0x40400000//存放inode，2MB大小，最多14979个文件。
#define FAT_START 0x40600000//2MB,flash起始为0x40200000,前2MB为kernel程序存放位置,从0x40400000开始的2MB存储fat表
#define DATA_START 0x40800000//4MB
#define DATA_END 0x60200000

#define PMC_ADDR 0x60200000

#define MONITOR_ADDR 0x60300000//3MB

// 统一的文件描述符约定: 0/1/2 是 TTY
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#define MAX_FD 16
#define FD_EMPTY 0
#define FD_TTY   1
#define FD_FILE  2

#define _NR_open 0
#define _NR_delete 1
#define _NR_write 2
#define _NR_read 3
#define _NR_create 4
#define _NR_powoff 7
#define _NR_exit 9
#define _NR_exec 10
#define _NR_send 11
#define _NR_recv 12
#define _NR_draw_label 13
#define _NR_draw_triangle 14
#define _NR_flush 15
#define _NR_open_monitor 16
#define _NR_close_monitor 17
#define _NR_malloc 18
#define _NR_free 19
#define _NR_finfo 20
#define _NR_draw_jpg 21
#define _NR_draw_png 22
#define _NR_accept 23
#define _NR_connect 24
#define _NR_close 25
#define _NR_spawn   26
#define _NR_waitpid 27
#define _NR_finoid  28
#define _NR_tty_frame 29
#define _NR_tty_size 30
#define _NR_flush_input 31

//interrupt num should same with hardware port
#define _NI_kbdown 2
#define _NI_timer 7

#define IO_CACHE 1024
#define FMT_STRING_SIZE 64

#define INT_MAX  2147483647
#define INT_MIN -2147483648

#define NULL 0

#endif // !_INI_H_
