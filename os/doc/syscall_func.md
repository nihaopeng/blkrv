|func|return|description|parameter||||
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|lproc|int status|列出进程|pcb* pcb|
|exec|int status|执行文件,优先级分123级,省略号为任意类型数量参数|const char* file_path|uint32_t* pid|uint32_t priority|...
|kill|int status|杀死进程|uint32_t pid|
|sleep|int status|睡眠,单位秒|uint32_t time|
|open|int status|打开文件,获取文件inodeid|const char* file_path|uint32_t* fid|
|read|int status|读取文件，一次性读取全部，限制文件大小为4MB|uint32_t fid|char* recv|
|write|int status|写文件，每次写都会清空文件|uint32_t fid|const char* write_data|uint32_t length|
|create|int status|创建文件|const char* file_path|char type|
|fstat|int status|查看文件inode|const char* file_path|inode* inode_t|
|update_inode|int status|更新inode|inode inode_t|
|print|int status|打印输出(%c%s)|const char* fmt|...|
|input|int status|阻塞输入()|const char* fmt|...|
|kbhit|int ifhit|非阻塞输入检查,ifhit代表是否存在键盘按下|
|getch|char ch|获取一个输入字符|
|socket|暂留
