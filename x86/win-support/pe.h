#ifndef __PE_H
#define __PE_H
struct PeHeader{
	uint32_t mMagic;
	uint16_t mMachine;
	uint16_t mNumberOfSections;
	uint32_t mTimeDateStamp;
	uint32_t mPointerToSymbolTable;
	uint32_t mNumberOfSymbols;
	uint16_t mSizeOfOptionalHeaders;
	uint16_t mCharacteristics;
}
#endif
