#include "fs.h"
#include <math.h>
// #include <vector>

int INE5412_FS::fs_format()
{
	union fs_block inode_block;
	union fs_block super_block;
	// disk->
	disk->read(0, super_block.data);
	for (int i = 0; i < super_block.super.ninodeblocks; i++) {
		cout << "ok!\n";
		disk->read(i+1, inode_block.data);
		for (int j = 0; j < INODES_PER_BLOCK; j++) {
			inode_block.inode[j].isvalid = 0;
		}
		disk->write(i + 1, inode_block.data);
	}
	cout << "ninodeblocks: " << ceil(disk->size()*0.1) << "\n";
	super_block.super.ninodeblocks = ceil(disk->size()*0.1);
	super_block.super.nblocks = disk->size();
	super_block.super.ninodes = 0;
	super_block.super.magic = FS_MAGIC;

	disk->write(0, super_block.data);
	
	// disk->read(0, super_block.data);
	// cout << super_block.super.ninodeblocks;
	return 1;
}

void INE5412_FS::fs_debug()
{
	union fs_block block;

	disk->read(0, block.data);

	cout << "superblock:\n";
	cout << "    " << (block.super.magic == FS_MAGIC ? "magic number is valid\n" : "magic number is invalid!\n");
 	cout << "    " << block.super.nblocks << " blocks\n";
	cout << "    " << block.super.ninodeblocks << " inode blocks\n";
	cout << "    " << block.super.ninodes << " inodes\n";

	union fs_block inode_block;

	for (int i = 0; i < block.super.ninodeblocks; i++) {
		disk->read(i+1, inode_block.data);
		for (int j = 0; j < INODES_PER_BLOCK; j++) {
			if (inode_block.inode[j].isvalid == 1) {
				cout << "inode " << (j+1)*(i)+(j) << ":\n";
				cout << "    " << "size: " << inode_block.inode[j].size << " bytes\n";
				cout << "    " << "direct blocks: ";
				for (const auto& direct_block: inode_block.inode[j].direct) {
					if (direct_block != 0) {
						cout << direct_block << " ";
					}
				}
				cout << "\n";
				int indirect = inode_block.inode[j].indirect;
				if (indirect != 0) {
					cout << "    " << "indirect block: " << indirect << "\n";
					cout << "    " << "indirect data blocks: ";
					union fs_block ind_block;
					disk->read(indirect, ind_block.data);
					for(const auto& block_pointer: ind_block.pointers) {
						if (block_pointer != 0) {
							cout << block_pointer << " ";
						}
					}
				}
				cout << "\n";
			}
		}
	}
	// fs_inode inode;
	// inode_load(2, &inode);
    // cout << inode.size << "\n";
    
}

int INE5412_FS::fs_mount()
{
	union fs_block block;
	disk->read(0, block.data);

	int nblocks = block.super.nblocks;
	int ninodeblocks = block.super.ninodeblocks;
	int bitmap[nblocks];

	for (int i = 0; i < nblocks; ++i) {
		if (i < ninodeblocks) {
			bitmap[i] = 1;
		} else {
			bitmap[i] = 0;
		} 
	}

	for (int i = 0; i < nblocks; ++i) {
		cout << i << ": " << bitmap[i] << "\n";
	}

	// FILE* bitmap_file = fopen("bitmap", "a+");

	// if (bitmap_file != nullptr) {
	// 	for () {

	// 	}
	// }
	return 1;
}

int INE5412_FS::fs_create()
{
	return 0;
}

int INE5412_FS::fs_delete(int inumber)
{
	return 0;
}

int INE5412_FS::fs_getsize(int inumber)
{
	return -1;
}

int INE5412_FS::fs_read(int inumber, char *data, int length, int offset)
{
	return 0;
}

int INE5412_FS::fs_write(int inumber, const char *data, int length, int offset)
{
	return 0;
}

void INE5412_FS::inode_load( int inumber, class fs_inode *inode ) {
    int i = inumber / INODES_PER_BLOCK + (inumber % INODES_PER_BLOCK != 0);
    union fs_block block;
    disk->read(i, block.data);
    i = (inumber % INODES_PER_BLOCK)-1;
    *inode = block.inode[i];
}
