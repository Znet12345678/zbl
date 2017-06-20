#ifndef __GEN_FS_FROM_FILE_H
#define __GEN_FS_FROM_FILE_H
struct fileent{
	int alloc;
	int sizeOfEnt;
	int sizeOfFile;
	int file_start_lba;
	int file_start_offset;
	short end;
};
#endif
