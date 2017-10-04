#include "lib.h"
#include "libpe.h"
int *loadPE(uint8_t *f){
	struct PeHeader *phdr = (struct PeHeader *)(f + 128);
	if(phdr->mMagic != 0x00004550){
		debug("LOADPE","Invalid magic number");
		return 0;
	}
	struct Pe32OptionalHeader *ohdr = (struct Pe32OptionalHeader *)(f + 128 + sizeof(*phdr));
	struct IMAGE_SECTION_HEADER *shdr = (struct IMAGE_SECTION_HEADER *)(f + 128 + sizeof(*phdr) + phdr->mSizeOfOptionalHeader);
	for(int i = 0; i < phdr->mNumberOfSections;i++){
		memcpy(shdr[i].mVirtualAddress,f + shdr[i].mPointerToRawData,shdr[i].mSizeOfRawData);
	}
	int * rpntr = ohdr->mAddressOfEntryPoint;
	return rpntr;
}
