#ifndef __GENFS_KSTUB
#define __GENFS_KSTUB
struct fd{
        uint8_t alloc;
        uint8_t name[80];
        uint32_t pos_lba;
        uint16_t pos_offset;
        uint8_t flags;
};

#endif
