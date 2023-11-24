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
			print_inode(((j + 1) * (i) + (j + 1)), &inode_block.inode[j]);
		}
	}
}

int INE5412_FS::fs_format()
{
	if (!is_mounted) {
		union fs_block inode_block;
		union fs_block super_block;
		
		disk->read(0, super_block.data);	// Read super block
		
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
		super_block.super.ninodeblocks = ceil(disk->size()*0.1);
		super_block.super.nblocks = disk->size();
		super_block.super.ninodes = 0;
		super_block.super.magic = FS_MAGIC;

		disk->write(0, super_block.data);

		return 1;
	}
	return 0;
}

int INE5412_FS::fs_mount()
{
	union fs_block block;
	disk->read(0, block.data);

	if(block.super.magic == FS_MAGIC) {
		int nblocks = block.super.nblocks;
		int ninodeblocks = block.super.ninodeblocks;
		bitmap = new int[nblocks]{0};

		for (int i = 0; i < ninodeblocks+1; ++i) {
			set_bitmap_block(i);
		}

		union fs_block inode_block;
		union fs_block ind_block;
		for (int i = 0; i < ninodeblocks; i++) {
			disk->read(i+1, inode_block.data);
			for (int j = 0; j < INODES_PER_BLOCK; j++) {
				if (inode_block.inode[j].isvalid == 1) {
					for (const auto& direct_block: inode_block.inode[j].direct) {
						set_bitmap_block(direct_block);
					}
					int indirect = inode_block.inode[j].indirect;
					if (indirect != 0) {
						set_bitmap_block(indirect);
						disk->read(indirect, ind_block.data);
						for(const auto& block_pointer: ind_block.pointers) {
							set_bitmap_block(block_pointer);
						}
					}
				}
			}
		}
		// for (int i = 0; i < nblocks; ++i) {
		// 	cout << i << ": " << bitmap[i] << "\n";
		// }
		is_mounted = true;
		return 1;
	}
	return 0;
}

int INE5412_FS::fs_create()
{
	if (is_mounted) {
		union fs_block inode_block;
		union fs_block super_block;

		disk->read(0, super_block.data);
		int inumber = 0;
		for (int i = 0; i < super_block.super.ninodeblocks; i++)
		{
			disk->read(i + 1, inode_block.data);
			for (int j = 0; j < INODES_PER_BLOCK; j++)
			{
				cout << (j + 1) * (i) + (j+1) << ": " << inode_block.inode[j].isvalid << "\n";
				if (!inode_block.inode[j].isvalid)
				{

					inumber = (j + 1) * (i) + (j+1);		// nao entendi mto bem como definir o inumber só sei q ele n pode ser 0
					inode_block.inode[j].isvalid = 1;
					inode_block.inode[j].size = 0;           // tem necessidade de definir isso?
					// limpando direct indirect  nao sei se isso deveria ser feito no format ou aqui
					for (int k = 0; k < POINTERS_PER_INODE; ++k) {
						if (inode_block.inode[j].direct[k] != 0) {
							inode_block.inode[j].direct[k] = 0;
						} 
					}
					// inode_block.inode[j].direct[POINTERS_PER_INODE] = {};
					inode_block.inode[j].indirect = 0;

					disk->write(i + 1, inode_block.data);
					return inumber;
				};
			}
		}
	}
	return 0;
}

int INE5412_FS::fs_delete(int inumber)
{
	if (is_mounted) {
		fs_inode inode;
		inode_load(inumber, &inode);
		if (inode.isvalid) {		
			for (int k = 0; k < POINTERS_PER_INODE; ++k) {
				if (inode.direct[k] != 0) {
					reset_bitmap_block(inode.direct[k]);
				}
			}
			if (inode.indirect != 0) {
				reset_bitmap_block(inode.indirect);
			}
			inode.isvalid = 0;
			inode_save(inumber, &inode);
			return 1;
		}
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
			// cout << "inode valid\n";
			int block_offset = offset / Disk::DISK_BLOCK_SIZE;  // disk block size
			// cout << "offset: " << offset << "\n";
			// cout << "block_offset: " << block_offset << "\n";
			for (int block_position = block_offset; block_position < POINTERS_PER_INODE; block_position++)
			{
				// cout << "Readed " << readed_bytes << " of " << length << " bytes" << "\n";
				// cout << "block_position: " << block_position << " : " << inode_target.direct[block_position] << "\n";
				
				if (readed_bytes < length && inode_target.direct[block_position] != 0) {
					if ((offset + readed_bytes + Disk::DISK_BLOCK_SIZE) < inode_target.size) {
						disk->read(inode_target.direct[block_position], &data[readed_bytes]);
						readed_bytes += Disk::DISK_BLOCK_SIZE;
					}
					else {
						// union fs_block block;
						disk->read(inode_target.direct[block_position], &data[readed_bytes]);
						// cout << "remaining_bytes: " << inode_target.size - (offset + readed_bytes) <<  "\n";
						readed_bytes += inode_target.size - (offset + readed_bytes);
						// lógica ta certa?
					}
				} else {
					return readed_bytes;
				}
			}

			if (inode_target.indirect != 0) {
				// cout << "indirect block: " << inode_target.indirect << "\n";
				union fs_block ind_block;
				disk->read(inode_target.indirect, ind_block.data);
				int p = 0;
				if (block_offset > POINTERS_PER_INODE)
					p = block_offset - POINTERS_PER_INODE;
				// cout << "pointer offset: " << p << "\n";
				for (; p < POINTERS_PER_BLOCK; p++)
				{
					// cout << "Readed " << readed_bytes << " of " << length << " bytes" << "\n";
					if (ind_block.pointers[p] != 0) {
						if (readed_bytes < length) {
							if ((offset + readed_bytes + Disk::DISK_BLOCK_SIZE) < inode_target.size) {
								disk->read(ind_block.pointers[p], &data[readed_bytes]);
								readed_bytes += Disk::DISK_BLOCK_SIZE;
							} else {
								disk->read(ind_block.pointers[p], &data[readed_bytes]);
								// cout << "remaining_bytes: " << inode_target.size - (offset + readed_bytes) <<  "\n";
								readed_bytes += inode_target.size - (offset + readed_bytes);
								// lógica ta certa?
							}
						} else {
							return readed_bytes;
						}
					} else {
						return readed_bytes;
					}
				}
			}
			return readed_bytes;
		}
	} else {
		cout << "ERROR: disk not mounted\n";
	}
	return 0;
}

int INE5412_FS::fs_write(int inumber, const char *data, int length, int offset)
{
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

// int INE5412_FS::read_block(int readed_bytes, int length, int block_pos, fs_inode *inode, char *data) {
// 	if (readed_bytes < length) {
// 		if ((readed_bytes + Disk::DISK_BLOCK_SIZE) < length) {
// 			disk->read(inode->direct[block_pos], data);
// 		} else {
// 			// lógica ta certa?
// 		}
// 	} else {
// 		return readed_bytes;
// 	}
// }

void INE5412_FS::show_bitmap() {
	for (int i = 0; i <= sizeof(bitmap); ++i) {
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
