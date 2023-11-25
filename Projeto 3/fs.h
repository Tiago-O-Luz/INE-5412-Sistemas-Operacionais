#ifndef FS_H
#define FS_H

#include "disk.h"

class INE5412_FS
{
public:
    static const unsigned int FS_MAGIC = 0xf0f03410;
    static const unsigned short int INODES_PER_BLOCK = 128;
    static const unsigned short int POINTERS_PER_INODE = 5;
    static const unsigned short int POINTERS_PER_BLOCK = 1024;

    class fs_superblock {
        public:
            unsigned int magic;
            int nblocks;
            int ninodeblocks;
            int ninodes;
    }; 

    class fs_inode {
        public:
            int isvalid;
            int size;
            int direct[POINTERS_PER_INODE];
            int indirect;
    };

    union fs_block {
        public:
            fs_superblock super;
            fs_inode inode[INODES_PER_BLOCK];
            int pointers[POINTERS_PER_BLOCK];
            char data[Disk::DISK_BLOCK_SIZE];
    };

public:

    INE5412_FS(Disk *d) {
        disk = d;
        is_mounted = false;
    }

    void fs_debug();
    int  fs_format();
    int  fs_mount();

    int  fs_create();
    int  fs_delete(int inumber);
    int  fs_getsize(int inumber);

    int  fs_read(int inumber, char *data, int length, int offset);
    int  fs_write(int inumber, const char *data, int length, int offset);
    
    void inode_load(int inumber, class fs_inode *inode);
    void inode_save(int inumber, class fs_inode *inode);

    void set_bitmap_block(int number);
    void reset_bitmap_block(int number);
    void set_inode_bitmap_info(fs_inode *inode);
    void prepare_inode(int inumber, fs_inode *inode);

    // int read_block(int readed_bytes, int length, int block_pos, fs_inode *inode, char *data);

    void show_bitmap(int array_size);
    void print_inode(int inumber, fs_inode *inode);
    int  get_inumber(int i, int j);

    int  get_block(int array_size);
    void clean_block(int bnumber);
    int  allocate_block(int *block, int nblocks, fs_inode *inode);
    void increase_byte_count(int cmp_value, int limit, int *byte_count);

private:
    Disk *disk;
    int *bitmap;
    bool is_mounted;
};

#endif