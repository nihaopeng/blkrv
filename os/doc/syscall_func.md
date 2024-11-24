|stat|func|return|description|parameter||||
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|√|exec||执行文件,优先级分123级,省略号为任意类型数量参数|const char* file_path|uint32_t* pid|uint32_t priority|...
|√|open|int status|打开文件,获取文件inodeid|const char* file_path|uint32_t* inode_id|int* status|
|√|exit||关闭进程|
|√|read||读取文件|uint32_t fid|char* recv|uint32_t start|uint32_t count|
|√|write||写文件，每次写都会清空文件|uint32_t inode_id|const char* write_data|uint32_t start|uint32_t count|
|√|create|int status|创建文件|const char* file_path|char type|uint32_t* inode_id|
|√|print||打印输出(%c%s%d)|const char* fmt|...|
|√|input||阻塞输入(%c%s%d)|const char* fmt|...|
|√|kbhit||非阻塞输入检查,ifhit代表是否存在键盘按下|int* ifhit|
|√|getch||获取一个输入字符|char* ch|
|√|send||发送字节|socket* sock|char* buf|uint32_t buf_length
|√|recv||接收响应|socket* sock|char* buf|uint32_t buf_length
|√|draw_label||绘制文字|point* p|char* str|color* c|int font
|√|draw_triangle||画三角形|point* p1|point* p2|point* p3|color* col
|√|flush||刷新缓冲区|