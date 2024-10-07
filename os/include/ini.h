#ifndef _INI_H_
#define _INI_H_

#define uint32_t unsigned int
#define uint16_t unsigned short
#define uint8_t unsigned char 


#define MAX_BLOCK 4096
#define BLOCK_SIZE 4*1023//byte

#define FILE_TABLE_ADDR 0x100200
#define FILE_DATA_ADDR 0x200200
//size ctrl is 4
#define SCREEN_CTRL_ADDR   0x30000000
#define SCREEN_CACHE1_ADDR (char *)0x30000004
#define SCREEN_CACHE2_ADDR (char *)0x30010004

#define MAX_NAME 128

#define _NR_open 0
#define _NR_delete 2
#define _NR_write 3
#define _NR_read 4
#define _NR_create 5
#define _NR_vprint 6

#define NULL 0

#endif // !_INI_H_


