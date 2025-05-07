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

//size ctrl is 4
#define SCREEN_CTRL_ADDR   0x10200000
#define SCREEN_CACHE1_ADDR 0x10200004
#define SCREEN_CACHE2_ADDR 0x10210004

#define GPU_ADDR_CACHE 0x20200000

#define NIC_IP_ADDR 0x30200000
#define NIC_PORT_ADDR 0x30200010
#define NIC_SDATA_LEN_ADDR 0x30200014
#define NIC_SDATA_ADDR 0x30200018
#define NIC_RDATA_LEN_ADDR 0x30600000
#define NIC_RDATA_ADDR 0x30600004

// #define FILE_TABLE_ADDR 0x40400000//2MB前2MB为kernel程序存放位置
// #define FILE_DATA_ADDR 0x40500000//3MB
#define INODE_START 0x40400000//存放inode，2MB大小，最多14979个文件。
#define FAT_START 0x40600000//2MB,flash起始为0x40200000,前2MB为kernel程序存放位置,从0x40400000开始的2MB存储fat表
#define DATA_START 0x40800000//4MB
#define DATA_END 0x60200000

#define PMC_ADDR 0x60200000

#define MONITOR_ADDR 0x60300000//3MB

#define _NR_open 0
#define _NR_delete 1
#define _NR_write 2
#define _NR_read 3
#define _NR_create 4
#define _NR_vprint 5
#define _NR_vgetch 6
#define _NR_powoff 7
#define _NR_kbhit 8
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
#define _NR_delete 21

//interrupt num should same with hardware port
#define _NI_kbdown 2
#define _NI_timer 9

#define IO_CACHE 1024
#define FMT_STRING_SIZE 64

#define INT_MAX  2147483647
#define INT_MIN -2147483648

#define NULL 0

#endif // !_INI_H_


