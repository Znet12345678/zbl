#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "libpe.h"
int main(int argc,char **argv){
	if(argc != 2)
		return -1;
	int fd = open(argv[1],O_RDONLY);
	int size = lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	uint8_t *buf = malloc(size);
	int bytes = read(fd,buf,size);
	if(!bytes)
		return -1;
	struct PeHeader *hdr = (struct PeHeader *)(buf + 128);
	struct Pe32OptionalHeader *ohdr = (struct Pe32OptionalHeader *)(buf + 128 + sizeof(*hdr));
	struct IMAGE_SECTION_HEADER *shdr = (struct IMAGE_SECTION_HEADER *)(buf + 128 + sizeof(*hdr) + hdr->mSizeOfOptionalHeader);
	printf("Main Header\n");
	printf("Magic 0x%.8x\nMachine %d\nNumber of Sections 0x%.4x\nTime 0x%.8x\nPointer to symbol table 0x%.8x\nNumber of symbols %d\nSize of optional header 0x%.4x\nCharacteristics 0x%.4x\n",hdr->mMagic,hdr->mMachine,hdr->mNumberOfSections,hdr->mTimeDateStamp,hdr->mPointerToSymbolTable,hdr->mNumberOfSymbols,hdr->mSizeOfOptionalHeader,hdr->mCharacteristics);
	close(fd);
	return 0;
}
