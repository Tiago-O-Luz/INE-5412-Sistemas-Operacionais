#include "fs.h"
#include <math.h>
// #include <vector>

void INE5412_FS::fs_debug()
{
	union fs_block block;

	// Read superblock
	disk->read(0, block.data);

	// Print superblock info
	cout << "superblock:\n";
	cout << "    " << (block.super.magic == FS_MAGIC ? "magic number is valid\n" : "magic number is invalid!\n");
 	cout << "    " << block.super.nblocks << " blocks\n";
	cout << "    " << block.super.ninodeblocks << " inode blocks\n";
	cout << "    " << block.super.ninodes << " inodes\n";

	union fs_block inode_block;

	// Iterate by every inode in every block
	for (int i = 0; i < block.super.ninodeblocks; i++) {
		disk->read(i+1, inode_block.data);	// Read inode block
		for (int j = 0; j < INODES_PER_BLOCK; j++) {
			print_inode(get_inumber(i, j), &inode_block.inode[j]);
		}
	}
}

int INE5412_FS::fs_format()
{
	if (!is_mounted) {
		union fs_block inode_block;
		union fs_block super_block;
		
		disk->read(0, super_block.data);	// Read superblock
		
		if (super_block.super.magic == FS_MAGIC) {
			// If disk has file system
			// Iterate by every inode in every block 
			for (int i = 0; i < super_block.super.ninodeblocks; i++) {
				disk->read(i+1, inode_block.data);
				for (int j = 0; j < INODES_PER_BLOCK; j++) {
					inode_block.inode[j].isvalid = 0;
				}
				disk->write(i + 1, inode_block.data);	// Write erased block
			}
		}
		// Set super block
		super_block.super.ninodeblocks = ceil(disk->size()*0.1);	// Aloccate 10% of disk to inode blocks
		super_block.super.nblocks = disk->size();					// Set number of blocks
		super_block.super.ninodes = 0;
		super_block.super.magic = FS_MAGIC;

		disk->write(0, super_block.data);	// Write new super block

		return 1;
	}
	cout << "ERROR: Disk already mounted\n";
	return 0;
}

int INE5412_FS::fs_mount()
{
	union fs_block block;
	disk->read(0, block.data);	// Read superblock

	if(block.super.magic == FS_MAGIC) {
		// If disk has file system
		int nblocks = block.super.nblocks;
		int ninodeblocks = block.super.ninodeblocks;

		bitmap = new int[nblocks]{0};	// Create new bitmap

		// Set to 1 superblock and inode blocks positions in the bitmap
		for (int i = 0; i < ninodeblocks+1; ++i) {
			set_bitmap_block(i);
		}

		union fs_block inode_block;
		// Iterate by every inode in every block 
		for (int i = 0; i < ninodeblocks; i++) {
			disk->read(i+1, inode_block.data);
			for (int j = 0; j < INODES_PER_BLOCK; j++) {
				set_inode_bitmap_info(&inode_block.inode[j]);	// Set inode on bitmap
			}
		};
		is_mounted = true;
		return 1;
	}
	cout << "ERROR: No file system found\n";
	return 0;
}

int INE5412_FS::fs_create()
{
	if (is_mounted) {
		union fs_block inode_block;
		union fs_block super_block;

		disk->read(0, super_block.data);	// Read superblock

		// Iterate by every inode in every block 
		for (int i = 0; i < super_block.super.ninodeblocks; i++)
		{
			disk->read(i + 1, inode_block.data);
			for (int j = 0; j < INODES_PER_BLOCK; j++)
			{
				// cout << get_inumber(i, j) << ": " << inode_block.inode[j].isvalid << "\n";
				if (!inode_block.inode[j].isvalid)
				{
					// // If inode is available
					int inumber = get_inumber(i, j);		// Find inumber
					prepare_inode(inumber, &inode_block.inode[j]);
					disk->write(i + 1, inode_block.data);	// Writes inode block data
					return inumber;
				};
			}
		}
		cout << "ERROR: can't create new inode. inode table full\n";
	} else {
		cout << "ERROR: disk must be mounted first!\n";
	}
	return 0;
}

int INE5412_FS::fs_delete(int inumber)
{
	if (is_mounted) {
		fs_inode inode;
		inode_load(inumber, &inode);	// Load inode
		if (inode.isvalid) {
			// Iterate by every direct pointer and reset blocks in bitmap
			for (int k = 0; k < POINTERS_PER_INODE; ++k) {
				if (inode.direct[k]) {
					reset_bitmap_block(inode.direct[k]);
				}
			}
			if (inode.indirect) {
				reset_bitmap_block(inode.indirect);
			}
			inode.isvalid = 0;
			inode_save(inumber, &inode);	// Save inode
			return 1;
		} else {
			cout << "ERROR: inode is not valid!\n";
		}
	} else {
		cout << "ERROR: disk must be mounted first!\n";
	}
	return 0;
}

int INE5412_FS::fs_getsize(int inumber)
{
	fs_inode inode_target;
	inode_load(inumber, &inode_target);
	if (inode_target.isvalid) {
		return inode_target.size;
	}
	return -1;
}

int INE5412_FS::fs_read(int inumber, char *data, int length, int offset)
{
	if (is_mounted) {
		int readed_bytes = 0;
		fs_inode inode_target;
		inode_load(inumber, &inode_target);
		
		if (inode_target.isvalid && offset < inode_target.size) {
			int block_offset = offset / Disk::DISK_BLOCK_SIZE;  // Block where the read process starts
			
			for (int block_position = block_offset; block_position < POINTERS_PER_INODE; block_position++) {
				// Iterate by every direct block
				if (readed_bytes < length && inode_target.direct[block_position] != 0) {
					// If length or end of file hasn't been reached continue to read

					// Adds block data to data buffer
					disk->read(inode_target.direct[block_position], &data[readed_bytes]);
					increase_byte_count((offset + readed_bytes), inode_target.size, &readed_bytes);

				} else {
					return readed_bytes;
				}
			}

			if (inode_target.indirect != 0) {
				union fs_block ind_block;
				disk->read(inode_target.indirect, ind_block.data);	// Read indirect block

				int p = 0;
				if (block_offset > POINTERS_PER_INODE) p = block_offset - POINTERS_PER_INODE;

				for (; p < POINTERS_PER_BLOCK; p++)
				{
					// Iterate by every pointer in indirect block
					if (ind_block.pointers[p] != 0) {
						// If hasn't reached end of file
						if (readed_bytes < length) {
							disk->read(ind_block.pointers[p], &data[readed_bytes]);
							increase_byte_count((offset + readed_bytes), inode_target.size, &readed_bytes);
						} else {
							break;
						}
					} else {
						return readed_bytes;
					}
				}
			}
			return readed_bytes;
		}
	} else {
		cout << "ERROR: disk must be mounted first!\n";
	}
	return 0;
}

int INE5412_FS::fs_write(int inumber, const char *data, int length, int offset)
{
	if (is_mounted) {
		union fs_block block;
		disk->read(0, block.data);	// Read superblock
		int nblocks = block.super.nblocks;

		int writed_bytes = 0;
		fs_inode inode_target;
		inode_load(inumber, &inode_target);
		if (inode_target.isvalid) {
			int block_offset = offset / Disk::DISK_BLOCK_SIZE;	// Block where the read process starts
			
			//clean_inode(block_offset, true, &inode_target);
			for (int block_position = block_offset; block_position < POINTERS_PER_INODE; block_position++) {
				// Iterate by every direct block

				if (writed_bytes < length) {
					// If length hasn't been reached continue to write
					
					// Allocates block if necessary
					int free_block = allocate_block(&inode_target.direct[block_position], nblocks, &inode_target);
					if (free_block == -1) {			
						// If no block is free returns with what was written
						inode_save(inumber, &inode_target);
						return writed_bytes;
					}
					
					// Write data buffer to in block
					disk->write(inode_target.direct[block_position], &data[writed_bytes]);
					increase_byte_count(writed_bytes, length, &inode_target.size);	// Set new size in inode
					increase_byte_count(writed_bytes, length, &writed_bytes);

				} else {
					clean_inode(block_position, true, &inode_target);
					inode_save(inumber, &inode_target);
					return writed_bytes;
				}
			}

			if (writed_bytes < length && inode_target.indirect == 0) { 
				// If inode doesn't have indirect block

				int free_block = allocate_block(&inode_target.indirect, nblocks, &inode_target);
				if (free_block == -1) {			// se nao tiver bloco livre só retorna oq foi escrito ate o momento
					inode_save(inumber, &inode_target);
					return writed_bytes;
				}
				clean_block(free_block, 0);
			}

			if (writed_bytes < length && inode_target.indirect != 0) {
				// If length hasn't been reached continue to write

				//Load indirect block
				union fs_block ind_block;
				disk->read(inode_target.indirect, ind_block.data);

				int p = 0;
				if (block_offset > POINTERS_PER_INODE) {
					p = block_offset - POINTERS_PER_INODE;
				}
				for (; p < POINTERS_PER_BLOCK; p++) {
					// Iterate by every indirect block
					if (writed_bytes < length) {
					// If length hasn't been reached continue to write

						// Allocates block if necessary
						int free_block = allocate_block(&ind_block.pointers[p], nblocks, &inode_target);
						if (free_block == -1) {
							// If no block is free break
							break;
						}

						disk->write(ind_block.pointers[p], &data[writed_bytes]);
						increase_byte_count(writed_bytes, length, &inode_target.size);
						increase_byte_count(writed_bytes, length, &writed_bytes);
					} else {
						break;
					}
				}
				clean_inode(p, false, &inode_target);
				// Saves inode and indirect block
				inode_save(inumber, &inode_target);
				disk->write(inode_target.indirect, ind_block.data);
			}
			inode_save(inumber, &inode_target);
			return writed_bytes;
		} else {
			cout << "ERROR: inode number not valid!\n";
		}
	} else {
		cout << "ERROR: disk must be mounted first!\n";
	}
	return 0;
}

void INE5412_FS::inode_load(int inumber, class fs_inode *inode ) {
    int i = inumber / INODES_PER_BLOCK + (inumber % INODES_PER_BLOCK != 0);
    union fs_block block;
    disk->read(i, block.data);
    i = (inumber % INODES_PER_BLOCK)-1;
    *inode = block.inode[i];
}

void INE5412_FS::inode_save(int inumber, fs_inode *inode)
{
    int i = inumber / INODES_PER_BLOCK + (inumber % INODES_PER_BLOCK != 0);
    union fs_block block;
    disk->read(i, block.data);
    int j = (inumber % INODES_PER_BLOCK)-1;
    block.inode[j] = *inode;
	disk->write(i, block.data);
}

void INE5412_FS::set_bitmap_block(int number) {
	bitmap[number] = 1;
}

void INE5412_FS::reset_bitmap_block(int number) {
	bitmap[number] = 0;
}

void INE5412_FS::set_inode_bitmap_info(fs_inode *inode) {
	if (inode->isvalid == 1) {
		// Set every direct block  
		for (const auto& direct_block: inode->direct) {
			set_bitmap_block(direct_block);
		}
		int indirect = inode->indirect;
		if (indirect != 0) {
			union fs_block ind_block;
			set_bitmap_block(indirect);				// Set indirect block  
			disk->read(indirect, ind_block.data);	// Read indirect block
			// Set every block in indirect block  
			for(const auto& block_pointer: ind_block.pointers) {
				set_bitmap_block(block_pointer);
			}
		}
	}
}

void INE5412_FS::prepare_inode(int inumber, fs_inode *inode) {
	inode->isvalid = 1;
	inode->size = 0;

	// Clean direct blocks
	for (int k = 0; k < POINTERS_PER_INODE; ++k) {
		inode->direct[k] = 0; 
	}
	// Clean indirect block (This implies that an allocated indirect block must be cleaned first)
	inode->indirect = 0;
}

void INE5412_FS::show_bitmap(int array_size) {
	for (int i = 0; i < array_size; ++i)
	{
		cout << i << ": " << bitmap[i] << "\n";
	}
}

void INE5412_FS::print_inode(int inumber, fs_inode *inode) {
	if (inode->isvalid == 1) {
		cout << "inode " << inumber << ":\n";
		cout << "    " << "size: " << inode->size << " bytes\n";
		cout << "    " << "direct blocks: ";
		// Iterate by every entry in direct block array
		for (const auto& direct_block: inode->direct) {
			if (direct_block != 0) {
				cout << direct_block << " ";
			}
		}
		cout << "\n";
		int indirect = inode->indirect;
		if (indirect != 0) {
			cout << "    " << "indirect block: " << indirect << "\n";
			cout << "    " << "indirect data blocks: ";
			union fs_block ind_block;
			disk->read(indirect, ind_block.data);	// Read indirect block
			// Iterate by every entry in indirect block
			for(const auto& block_pointer: ind_block.pointers) {
				if (block_pointer != 0) {
					cout << block_pointer << " ";
				}
			}
		}
		cout << "\n";
	}
}

int INE5412_FS::get_inumber(int i, int j) {
	return (j + 1) * (i) + (j + 1);
}

int INE5412_FS::get_block(int array_size) {
	for (int i = 1; i < array_size; i++) {
		if (bitmap[i] == 0) {
			return i;
		}
	}
	return -1;
}

void INE5412_FS::clean_block(int bnumber, int position) {
	union fs_block block;
	disk->read(bnumber, block.data);

	for (; position < POINTERS_PER_BLOCK; ++position) {
		int pointer_block = block.pointers[position];
		if (pointer_block != 0) {
			reset_bitmap_block(pointer_block);
		}
		block.pointers[position] = 0;
	}

	disk->write(bnumber, block.data);
}

int INE5412_FS::allocate_block(int *block, int nblocks, fs_inode *inode) {
	if (*block == 0) {
		int free_block = get_block(nblocks);
		if (free_block != -1) {
			cout << "Free block: " << free_block << "\n";
			*block = free_block;
			set_bitmap_block(free_block);
			return free_block;
		} else { 											// mexi aqui 
			return -1;
		}
		return free_block;
	}
	return 0;

}

void INE5412_FS::increase_byte_count(int cmp_value, int limit, int *byte_count) {
	if ((cmp_value + Disk::DISK_BLOCK_SIZE) < limit) {
		*byte_count += Disk::DISK_BLOCK_SIZE;	// Count block size
	} else {
		*byte_count += limit - cmp_value;	// Count only the remaining bytes
	}
}

void INE5412_FS::clean_inode(int position, bool isDirect, fs_inode *inode) {
	int indirect_inode = inode->indirect;
	if (isDirect) {
		for (; position < POINTERS_PER_INODE; position++) {
			if (inode->direct[position]) {
				reset_bitmap_block(inode->direct[position]);
			}
			inode->direct[position] = 0;
		}
		if (indirect_inode != 0){
			clean_block(indirect_inode, 0);
		}
	} else if (indirect_inode != 0) {
		clean_block(indirect_inode, position);
	}
}
