#include "console.h"

void deleteSingle(char* buffer, int pos){
    int len = str_len(buffer);
    for(int i = pos; i < len -1; i++){
        buffer[i] = buffer[i+1];
    }
    buffer[len - 1] = '\0';
}

void insertMiddle(char* buffer, int pos, char c){
    int len = str_len(buffer);
    for(int i = len; i > pos; i--){
        buffer[i] = buffer[i-1];
    }
    buffer[pos] = c;
}

void init_console(){
    int read_esc = 0;
    int is_escape = 0;
    print("%s", "welcome to blkrv\n");

    char c;
    int buffer_size = 1024;
    int buffer_pos = 0;
    int buffer_current_len = 0;
    char buffer[buffer_size];

    char promopt[] = {"[root@blkrv /]# \0"};

    for(int i = 0; i < 1024; i++){
        buffer[i] = 0;
    }

    print("%s", promopt);

    while (1)
    {
        vgetchk(&c);
        if (c == 0){
            continue;
        }
        // print("%d ", c);
        // printk("%c", c);
        // continue;

        if (c == 27){
            read_esc = 1;
            continue;
        }

        if (read_esc == 1){
            if (c == 91){
                is_escape = 1;
                continue;
            }else{
                read_esc = 0;
            }
        }

        if (is_escape == 1){
            is_escape = 0;
            read_esc = 0;
            if (c == 'D'){
                if (buffer_pos > 0){
                    print("\x1B[D");
                    buffer_pos--;
                }
                continue;
            }

            if (c == 'C'){
                if (buffer_pos < buffer_current_len){
                    print("\x1B[C");
                    buffer_pos++;
                }
                continue;
            }

            if (c == 'H'){
                buffer_pos = 0;
                print("\x1B[%dG", str_len(promopt) + 1);
                continue;
            }

            if (c == 'F'){
                buffer_pos = buffer_current_len;
                print("\x1B[%dG", str_len(promopt) + 1 + buffer_current_len);
                continue;
            }
            continue;
        }

        if (c >= 32 && c <= 126){
            if (buffer_pos != buffer_current_len){
                insertMiddle(buffer, buffer_pos, c);
                print("\x1B[s\r%s%s\x1B[u\x1B[C", promopt, buffer);
            }else{
                buffer[buffer_pos] = c;
                print("\r%s%s", promopt, buffer);
            }
            buffer_pos++;
            buffer_current_len++;
        }

        if (c == 127){
            if (buffer_current_len <= 0){
                continue;
            }

            if (buffer_pos <= 0){
                continue;
            }
            buffer_current_len--;
            buffer_pos--;
            deleteSingle(buffer, buffer_pos);
            print("\x1B[s\r\x1B[K%s%s\x1B[u\x1B[D", promopt, buffer);
        }

        if (c == 10){
            print("\n%s\n%s", buffer, promopt);
            for(int i = 0; i < buffer_current_len; i++){
                buffer[i] = 0;
            }
            buffer_current_len = 0;
            buffer_pos = 0;
        }
    }
    
}
