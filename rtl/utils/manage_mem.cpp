#include <cstring>
#include <fstream>
#include <cstdint>

int main(int argc, char** argv) {
    std::fstream src_file,tar_file;
    uint64_t src_start,src_end,tar_start,tar_end;
    if(argc<2){
        printf("this is help");
        return 0;
    }
    for(int i=1;i<argc;i++){
        // printf("%s\n",argv[i]);
        if(!strcmp(argv[i],"-h")){
            printf("this is help");
            return 0;
        }else if(!strcmp(argv[i],"-src")){
            src_file.open(argv[i+1],std::ios::in|std::ios::binary|std::ios::out);
            i++;
        }else if(!strcmp(argv[i],"-tar")){
            tar_file.open(argv[i+1],std::ios::out|std::ios::binary|std::ios::in);
            i++;
        }else if(!strcmp(argv[i],"-src_start")){
            src_start=atoi(argv[i+1]);
            i++;
        }else if(!strcmp(argv[i],"-src_end")){
            src_end=atoi(argv[i+1]);
            i++;
        }else if(!strcmp(argv[i],"-tar_start")){
            tar_start=atoi(argv[i+1]);
            i++;
        }else if(!strcmp(argv[i],"-tar_end")){
            tar_end=atoi(argv[i+1]);
            i++;
        }else{
            printf("unknown option:%s\n",argv[i]);
        }
    }
    if(!src_file||!tar_file){
        printf("can not open file\n");
        return 0;
    }else{
        uint32_t data;
        src_file.seekg(34);
        src_file.read(reinterpret_cast<char*>(&data), sizeof(data));
        printf("the first data:%x\n",data);
    }
    printf("src_start:%lu;src_end:%lu;tar_start%lu;tar_end%lu\n",src_start,src_end,tar_start,tar_end);
    if(tar_end-tar_start!=src_end-src_start){
        printf("wrong number of byte");
        return 0;
    }
    if(src_file&&tar_file){
        uint64_t len=tar_end-tar_start;
        for(uint64_t i=0;i<=len;i++){
            printf("%lu/%lu\r",i,len);
            uint8_t data;
            src_file.seekg(src_start+i);
            src_file.read(reinterpret_cast<char*>(&data), sizeof(data));
            // printf("%d",data);
            tar_file.seekp(tar_start+i);
            tar_file.write(reinterpret_cast<const char*>(&data), sizeof(data));
            tar_file.flush();
        }
    }
    printf("\n");
    return 0;
}