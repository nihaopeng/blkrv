#include <cstring>
#include <fstream>
#include <cstdint>
#include <vector>

void copy_file_range(const char* src_path, size_t src_start, size_t length,
    const char* dst_path, size_t dst_start) {

    printf("src_file:%s;dst_file:%s;src_start:%lu;dst_start:%lu;length:%lu\n",src_path,dst_path,src_start,dst_start,length);
    // 打开源文件（二进制模式）
    std::ifstream src(src_path, std::ios::binary);
    src.seekg(src_start);

    // 打开目标文件（二进制+读写模式，保留原有内容）
    std::fstream dst(dst_path, std::ios::binary | std::ios::in | std::ios::out);
    dst.seekp(dst_start);

    // 设置缓冲区（建议 4KB~1MB）
    const size_t buffer_size = 4096;
    std::vector<char> buffer(buffer_size);

    size_t remaining = length;
    while (remaining > 0) {
        size_t read_size = std::min(buffer_size, remaining);
        src.read(buffer.data(), read_size);
        size_t actual_read = src.gcount();  // 实际读取的字节数

        if (actual_read == 0) break;  // 源文件结束

        dst.write(buffer.data(), actual_read);
        remaining -= actual_read;

        if (actual_read < read_size) break;  // 源文件不足
    }
    // dst.flush();
    // 确保目标文件足够大（可选）
    dst.seekp(0, std::ios::end);
    if (dst.tellp() < dst_start + length) {
        dst.seekp(dst_start + length - 1);
        dst.put('\0');  // 扩展文件
    }

    src.close();
    dst.close();
}

int main(int argc, char** argv) {
    // std::fstream src_file,tar_file;
    char* src_file;
    char* tar_file;
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
            src_file=argv[i+1];
            i++;
        }else if(!strcmp(argv[i],"-tar")){
            tar_file=argv[i+1];
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
    if(tar_end-tar_start!=src_end-src_start){
        printf("wrong number of byte");
        return 0;
    }
    copy_file_range(src_file,src_start,src_end-src_start,tar_file,tar_start);
    // if(src_file&&tar_file){
    //     uint64_t len=tar_end-tar_start;
    //     for(uint64_t i=0;i<=len;i++){
    //         printf("%lu/%lu\r",i,len);
    //         uint8_t data;
    //         src_file.seekg(src_start+i);
    //         src_file.read(reinterpret_cast<char*>(&data), sizeof(data));
    //         // printf("%d",data);
    //         tar_file.seekp(tar_start+i);
    //         tar_file.write(reinterpret_cast<const char*>(&data), sizeof(data));
    //         tar_file.flush();
    //     }
    // }
    printf("\n");
    return 0;
}