#ifndef _INI_H_
#define _INI_H_

#define uint32_t unsigned int
#define uint16_t unsigned short
#define uint8_t unsigned char 


#define MAX_BLOCK 64512//mem size div block size
#define FILE_NUM 4096
#define BLOCK_SIZE 4*1024//byte,last 4 byte used as ptr to next block
#define MAX_NAME 128
#define NULL 0
#define BLOCK_USED 1
#define INODE_SIZE 256
#define MAX_PID_NUM 64
#define PROC_MEM 0x00800000//8MB
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

#define FILE_TABLE_ADDR 0x40400000//2MB40400000
#define FILE_DATA_ADDR 0x40500000//3MB

#define PMC_ADDR 0x60200000//3MB

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
#define _NR_send 10
#define _NR_recv 11
#define _NR_draw_label 12
#define _NR_draw_triangle 13
#define _NR_flush 14

//interrupt num should same with hardware port
#define _NI_kbdown 2

#define IO_CACHE 1024*1024
#define FMT_STRING_SIZE 64

#define INT_MAX  2147483647
#define INT_MIN -2147483648

#endif // !_INI_H_


