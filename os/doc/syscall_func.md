|stat|func|return|description|parameter||||
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
||lproc|int status|列出进程|pcb* pcb|
||exec|int status|执行文件,优先级分123级,省略号为任意类型数量参数|const char* file_path|uint32_t* pid|uint32_t priority|...
||kill|int status|杀死进程|uint32_t pid|
||sleep|int status|睡眠,单位秒|uint32_t time|
||open|int status|打开文件,获取文件inodeid|const char* file_path|uint32_t* inode_id|int* status|
||read|int status|读取文件|uint32_t fid|char* recv|uint32_t start|uint32_t count|
||write|int status|写文件，每次写都会清空文件|uint32_t fid|const char* write_data|uint32_t count|
||create|int status|创建文件|const char* file_path|char type|uint32_t* inode_id|
||fstat|int status|查看文件inode|const char* file_path|inode* inode_t|
||update_inode|int status|更新inode|inode inode_t|
|√|print|int status|打印输出(%c%s%d)|const char* fmt|...|
|√|input|int status|阻塞输入(%c%s%d)|const char* fmt|...|
|√|kbhit|int status|非阻塞输入检查,ifhit代表是否存在键盘按下|int* ifhit|
|√|getch|int status|获取一个输入字符|char* ch|
||socket|暂留
