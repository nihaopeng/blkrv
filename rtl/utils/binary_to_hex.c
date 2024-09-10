#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

/*long long bFileSize(FILE *bfile)
{
    long long bfile_size=0;
    if(fseek(bfile,0,SEEK_END)!=0)
    {
        perror("error");
        return -1;
    }
    bfile_size=ftell(bfile);
    if(fseek(bfile,-(bfile_size),SEEK_CUR)!=0)
    {
        perror("error");
        return -1;
    }
    return bfile_size;
}*/


int main(int argc,char *argv[])
{
    if(argc!=4)
    {
        printf("should include three parameters!\n");
        return -1;
    }

    FILE *bfile;
    char *buffer;
    long len;
    int start_byte=atoi(argv[2]);
    int end_byte=atoi(argv[3]);
    len=end_byte-start_byte+1;
//    printf("len=%ld\n",len);

    bfile=fopen(argv[1],"rb+");
    if(bfile==NULL)
    {
        perror("open failed");
        return -1;
    }

 //   len=bFileSize(bfile);
    if(fseek(bfile,start_byte,SEEK_SET)!=0)
    {
        perror("error");
        fclose(bfile);
        return -1;
    }

    buffer=(char *)malloc(len);
    if(buffer==NULL)
    {
        printf("allocation failure\n");
        return -1;
    }
    long count=fread(buffer,1,len,bfile);
    if(count!=len)
    {
        perror("error");
        fclose(bfile);
        return -1;
    }
//    printf("%ld\n", count);
    for(int i=0;i<count;i++)
    {
        printf("%02X ",(unsigned char)buffer[i]);
    }
    free(buffer);
    fclose(bfile);
    return 0;
}