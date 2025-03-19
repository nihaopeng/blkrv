#include "console.h"

int history_num = 100;
int buffer_size = 4096;

void deleteSingle(char buffer[], int pos){
    int len = str_len(buffer);
    for(int i = pos; i < len -1; i++){
        buffer[i] = buffer[i+1];
    }
    buffer[len - 1] = '\0';
}


void insertMiddle(char buffer[], int pos, char c){
    int len = str_len(buffer);
    for(int i = len; i > pos; i--){
        buffer[i] = buffer[i-1];
    }
    buffer[pos] = c;
}


void pushHistory(char history[][buffer_size], char buffer[]){
    for (int i = history_num - 1; i > 0; i--) {
        int len = str_len(history[i - 1]) + 1;
        for (int j = 0; j < len; j++){
            history[i][j] = history[i - 1][j];
        }
    }

    int len = str_len(buffer) + 1;
    for (int i = 0; i < len; i++){
        history[0][i] = buffer[i];
    }
}


void recoverHistory(char history[][buffer_size], char buffer[], int history_pos){
    int len = str_len(history[history_pos]);
    for (int i = 0; i < len; i++){
        buffer[i] = history[history_pos][i];
    }
    buffer[len] = '\0';
}

void init_console(){
    int read_esc = 0;
    int is_escape = 0;
    history_num = 10;
    buffer_size = 1024;

    int buffer_pos = 0;
    int buffer_current_len = 0;
    char buffer[buffer_size];
    for (int i = 0; i < buffer_size; i++){
        buffer[i] = '\0';
    }

    int history_pos = -1;
    int history_current_num = 0;
    char history[history_num][buffer_size];
    for (int i = 0; i < history_num; i++) {
        for (int j = 0; j < buffer_size; j++){
            history[i][j] = '\0';
        }
    }

    print("%s", "welcome to blkrv\n");
    char promopt[] = {"[root@blkrv /]# \0"};
    print("%s", promopt);

    char c;
    while (1)
    {
        vgetchk(&c);
        if (c == 0){
            continue;
        }

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
                if (history_pos != -1){
                    recoverHistory(history, buffer, history_pos);
                    buffer_pos = str_len(buffer);
                    buffer_current_len = buffer_pos;
                    print("\r\x1B[K%s%s", promopt, buffer);
                    history_pos = -1;
                }

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

            if (c == 'A'){
                history_pos++;
                if (history_pos > history_current_num - 1){
                    history_pos--;
                    continue;
                }
                print("\r\x1B[K%s%s", promopt, history[history_pos]);
                continue;
            }

            if (c == 'B'){
                history_pos--;
                if (history_pos <= -1){
                    history_pos = -1;
                    print("\r\x1B[K%s%s", promopt, buffer);
                    continue;
                }
                print("\r\x1B[K%s%s", promopt, history[history_pos]);
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
            if (buffer_current_len >= buffer_size - 1){
                continue;
            }

            if (history_pos != -1){
                recoverHistory(history, buffer, history_pos);
                buffer_pos = str_len(buffer);
                buffer_current_len = buffer_pos;
                print("\r\x1B[K%s%s", promopt, buffer);
                history_pos = -1;
            }

            if (buffer_pos != buffer_current_len){
                insertMiddle(buffer, buffer_pos, c);
                print("\x1B[s\r%s%s\x1B[u\x1B[C", promopt, buffer);
            }else{
                buffer[buffer_pos] = c;
                print("\r\x1B[K%s%s", promopt, buffer);
            }
            buffer_pos++;
            buffer_current_len++;
        }

        if (c == 127){
            if (history_pos != -1){
                recoverHistory(history, buffer, history_pos);
                buffer_pos = str_len(buffer);
                buffer_current_len = buffer_pos;
                print("\r\x1B[K%s%s", promopt, buffer);
                history_pos = -1;
            }

            if (buffer_current_len <= 0){
                continue;
            }

            if (buffer_pos <= 0){
                continue;
            }

            deleteSingle(buffer, buffer_pos);
            buffer_current_len--;
            buffer_pos--;
            print("\x1B[s\r\x1B[K%s%s\x1B[u\x1B[D", promopt, buffer);
        }

        if (c == 10){
            if (str_len(buffer) == 0){
                print("\n%s", promopt);
                continue;
            }

            buffer[buffer_current_len] = '\0';

            pushHistory(history, buffer);
            if (history_current_num < history_num){
                history_current_num++;
            }
            
            print("\n%s%s\n%s", promopt, buffer, promopt);

            for (int i = 0; i < buffer_current_len; i++){
                buffer[i] = '\0';
            }

            buffer_current_len = 0;
            buffer_pos = 0;
        }
    }
    
}
