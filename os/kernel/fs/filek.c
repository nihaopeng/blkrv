#include "file.h"

void* memcpy(void *dest, const void *src, uint32_t n) {
    char *d = dest;
    const char *s = src;

    // 逐字节复制内存内容
    for (uint32_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}

// 辅助函数：获取inode指针
inode* get_inode_ptr(uint32_t inode_id) {
    return (inode*)(INODE_START + inode_id * sizeof(inode));
}

// 获取FAT条目
uint32_t get_fat_entry(uint32_t block_num) {
    uint32_t* fat = (uint32_t*)FAT_START;
    return fat[block_num];
}

void set_fat_entry(uint32_t block_num, uint32_t value) {
    uint32_t* fat = (uint32_t*)FAT_START;
    fat[block_num] = value;
}

// 查找文件最后一个块
uint32_t find_last_block(inode* node) {
    if (node->start_block == 0xFFFFFFFF) return 0xFFFFFFFF;
    uint32_t current = node->start_block;
    while (get_fat_entry(current) != 0xFFFFFFFF)
        current = get_fat_entry(current);
    return current;
}

// 查找空闲块
int find_free_block() {
    uint32_t total_blocks = (DATA_END - DATA_START) / BLOCK_SIZE;
    for (uint32_t i = 0; i < total_blocks; i++)
        if (get_fat_entry(i) == 0) return i;
    return -1;
}

// 分配空闲inode
int allocate_inode(uint32_t* inode_id) {
    uint32_t max_inodes = (FAT_START - INODE_START) / sizeof(inode);
    for (*inode_id = 0; *inode_id < max_inodes; (*inode_id)++) {
        inode* node = get_inode_ptr(*inode_id);
        if (node->type == 0) return 0;
    }
    return -1;
}

// 通过完整路径查找inode
int find_inode_by_path(const char* path, uint32_t* id) {
    uint32_t max_inodes = (FAT_START - INODE_START) / sizeof(inode);
    for (uint32_t i = 0; i < max_inodes; i++) {
        inode* node = get_inode_ptr(i);
        if (node->type != 0 && !str_cmp(node->file_name, path) == 0) {
            *id = i;
            return 1;
        }
    }
    return 0;
}

// 分割路径为父目录和文件名
int split_parent_and_filename(const char* path, char* parent, char* filename) {
    const char* end = strrchr(path, '/');
    if (!end) return 0;
    strncpy(parent, path, end - path);
    parent[end - path] = '\0';
    str_cpy(end + 1,filename);
    // printk("parent:%s\n",parent);
    // printk("filename:%s\n",filename);
    return 1;
}

// 在目录中添加条目
int add_directory_entry(uint32_t dir_inode_id, const char* name, uint32_t entry_inode, uint8_t type) {
    inode* dir = get_inode_ptr(dir_inode_id);
    if (dir->type != DIR_TYPE) return -1;

    uint32_t block = dir->start_block;
    uint32_t prev_block = 0xFFFFFFFF;

    while (1) {
        if (block == 0xFFFFFFFF) {
            // 分配新块
            int new_block = find_free_block();
            if (new_block == -1) return -2;
            if (prev_block == 0xFFFFFFFF) {
                dir->start_block = new_block;
            } else {
                set_fat_entry(prev_block, new_block);
            }
            set_fat_entry(new_block, 0xFFFFFFFF);
            block = new_block;
            memset_s((void*)(DATA_START + block * BLOCK_SIZE), 0, BLOCK_SIZE);
        }

        dir_entry* entries = (dir_entry*)(DATA_START + block * BLOCK_SIZE);
        for (int i = 0; i < BLOCK_SIZE / sizeof(dir_entry); i++) {
            if (entries[i].inode_id == 0) {
                strncpy(entries[i].name, name, MAX_NAME);
                entries[i].inode_id = entry_inode;
                entries[i].type = type;
                return 0;
            }
        }

        prev_block = block;
        block = get_fat_entry(block);
    }
}

// 创建文件或目录（支持自动创建父目录）
int createk(char* file_path, char type, uint32_t* inode_id) {
    uint32_t existing_id;
    if (find_inode_by_path(file_path, &existing_id)) return -1;

    char parent_path[MAX_PATH_LEN], filename[MAX_NAME];
    if (!split_parent_and_filename(file_path, parent_path, filename)) return -2;

    // 递归创建父目录
    uint32_t parent_inode;
    if (str_len(parent_path) > 0 && !find_inode_by_path(parent_path, &parent_inode)) {
        int res = createk(parent_path, DIR_TYPE, &parent_inode);
        if (res != 0) return res;
    } else if (str_len(parent_path) == 0) {
        parent_inode = 0; // 根目录
    }

    inode* parent = get_inode_ptr(parent_inode);
    if (parent->type != DIR_TYPE) return -3;

    // 创建新inode
    uint32_t new_id;
    if (allocate_inode(&new_id) != 0) return -4;
    inode* new_node = get_inode_ptr(new_id);
    str_cpy(file_path,new_node->file_name);
    new_node->type = type;
    new_node->size = 0;
    new_node->start_block = 0xFFFFFFFF;

    // 添加目录条目到父目录
    
    if (add_directory_entry(parent_inode, filename, new_id, type) != 0) {
        new_node->type = 0;
        return -5;
    }

    *inode_id = new_id;
    return 0;
}

// 打开文件（返回inode_id）
int openk(char* file_path, uint32_t* inode_id) {
    return find_inode_by_path(file_path, inode_id) ? 0 : -1;
}

// 读取文件
int readk(uint32_t inode_id, char* buf, uint32_t start, uint32_t count) {
    inode* node = get_inode_ptr(inode_id);
    if (node->type == 0 || start >= node->size) return 0;

    uint32_t copied = 0;
    uint32_t block_idx = start / BLOCK_SIZE;
    uint32_t offset = start % BLOCK_SIZE;
    uint32_t current_block = node->start_block;

    // 定位到起始块
    for (uint32_t i = 0; i < block_idx && current_block != 0xFFFFFFFF; i++)
        current_block = get_fat_entry(current_block);

    while (count > 0 && current_block != 0xFFFFFFFF) {
        char* data = (char*)(DATA_START + current_block * BLOCK_SIZE);
        uint32_t chunk = (BLOCK_SIZE - offset < count) ? BLOCK_SIZE - offset : count;
        memcpy(buf + copied, data + offset, chunk);
        copied += chunk;
        count -= chunk;
        offset = 0;
        current_block = get_fat_entry(current_block);
    }
    return copied;
}

// 写入文件
int writek(uint32_t inode_id, char* buf, uint32_t start, uint32_t count) {
    inode* node = get_inode_ptr(inode_id);
    if (node->type == 0) return -1;

    // 扩展文件大小并分配块
    uint32_t required_size = start + count;
    if (required_size > node->size) {
        uint32_t new_blocks = (required_size + BLOCK_SIZE - 1) / BLOCK_SIZE - (node->size + BLOCK_SIZE - 1) / BLOCK_SIZE;
        uint32_t last_block = find_last_block(node);

        for (uint32_t i = 0; i < new_blocks; i++) {
            int free_block = find_free_block();
            if (free_block == -1) return -1;
            if (last_block == 0xFFFFFFFF) {
                node->start_block = free_block;
            } else {
                set_fat_entry(last_block, free_block);
            }
            set_fat_entry(free_block, 0xFFFFFFFF);
            last_block = free_block;
        }
        node->size = required_size;
    }

    // 写入数据
    uint32_t written = 0;
    uint32_t block_idx = start / BLOCK_SIZE;
    uint32_t offset = start % BLOCK_SIZE;
    uint32_t current_block = node->start_block;

    for (uint32_t i = 0; i < block_idx && current_block != 0xFFFFFFFF; i++)
        current_block = get_fat_entry(current_block);

    while (count > 0 && current_block != 0xFFFFFFFF) {
        char* data = (char*)(DATA_START + current_block * BLOCK_SIZE);
        uint32_t chunk = (BLOCK_SIZE - offset < count) ? BLOCK_SIZE - offset : count;
        memcpy(data + offset, buf + written, chunk);
        written += chunk;
        count -= chunk;
        offset = 0;
        current_block = get_fat_entry(current_block);
    }
    return written;
}

// 获取文件信息
int get_file_info(char* file_path, inode* out_info) {
    uint32_t id;
    if (!find_inode_by_path(file_path, &id)) return -1;
    memcpy(out_info, get_inode_ptr(id), sizeof(inode));
    return 0;
}

int init_fs(){
    uint32_t root_id;
    if (!find_inode_by_path("/", &root_id)) {
        printk("find /\n");
        createk("/", DIR_TYPE, &root_id);
        printk("create /\n");
        inode* root = get_inode_ptr(root_id);
        root->start_block = find_free_block();
        set_fat_entry(root->start_block, 0xFFFFFFFF);
        add_directory_entry(root_id, ".", root_id, DIR_TYPE);
        add_directory_entry(root_id, "..", root_id, DIR_TYPE);
    }
    createk("/include",DIR_TYPE,&root_id);
    printk("create /include\n");
    createk("/tmp",DIR_TYPE,&root_id);
    printk("create /tmp\n");
    createk("/tmp/test.bin",FILE_TYPE,&root_id);
    printk("create /tmp/test.bin\n");
}