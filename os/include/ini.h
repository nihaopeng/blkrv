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
//size is 1
#define KEYBOARD_CACHE_ADDR 0x20000000

//size ctrl is 4
#define SCREEN_CTRL_ADDR   0x30000000
#define SCREEN_CACHE1_ADDR 0x30000004
#define SCREEN_CACHE2_ADDR 0x30010004

#define NIC_IP_ADDR 0x50000000
#define NIC_PORT_ADDR 0x50000010
#define NIC_SDATA_LEN_ADDR 0x50000014
#define NIC_SDATA_ADDR 0x50000018
#define NIC_RDATA_LEN_ADDR 0x50400000
#define NIC_RDATA_ADDR 0x50400004

#define FILE_TABLE_ADDR 0x60200000//2MB
#define FILE_DATA_ADDR 0x60300000//3MB

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

//interrupt num should same with hardware port
#define _NI_kbdown 2

#define IO_CACHE 1024*1024
#define FMT_STRING_SIZE 64

#define INT_MAX  2147483647
#define INT_MIN -2147483648

#endif // !_INI_H_


