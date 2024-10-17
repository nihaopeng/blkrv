#include "drivers.h"


char in_cache[IO_CACHE];
int in_cache_mutex=0;
int in_cache_frontp=0,in_cache_backp=0;

int vgetch_i(){
    if(in_cache_frontp!=in_cache_backp){
        char ch=in_cache[in_cache_frontp];
        in_cache_frontp++;
        in_cache_frontp=mod(in_cache_frontp,1024*1024);
        return ch;
    }else{
        return -1;
    }
}