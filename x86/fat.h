#ifndef __FAT_H
#define __FAT_H
#include <stdint.h>
struct __fat_tbl{
	uint32_t SectorsPerFat;
	uint16_t flags;
	uint16_t vnum;
	uint32_t root_cluster;
	uint16_t fsinfo_sect;
	uint16_t bbs_sect;
	uint8_t resv1[12];
	uint8_t drive_num;
	uint8_t resv2;
	uint8_t sig;
	uint32_t serial;
	uint8_t lbl[11];
	uint8_t sysident[8];
	uint8_t boot_code[420];
	uint16_t boot_sig;
};
struct __fat_dent{
	uint8_t name[11];
	uint8_t attrib;
	uint8_t ntresv1;
	uint8_t creationtime;
	uint16_t time_crt;
	uint16_t date_crt;
	uint16_t last_accessed;
	uint16_t high_cluster_num;
	uint16_t last_modify;
	uint16_t last_modify_date;
	uint16_t low_cluster_num;
	uint32_t size;
};
struct long_name{
	uint16_t order;
	uint16_t c[5];
	uint8_t attrib;
	uint8_t type;
	uint8_t check;
	uint16_t c2[6];
	uint16_t zero;
	uint16_t c3[2];
};
struct __fat_dat{
	int total_sectors;
	int fat_size;
	int root_dir_sectors;
	int first_data_sector;
	int first_fat_sector;
	int data_sectors;
	int total_clusters;
};
struct __fat_extBS{
	uint32_t table_size_32;
	uint16_t extended_flags;
	uint16_t fat_version;
	uint32_t root_cluster;
	uint16_t fat_info;
	uint16_t backup_sector;
	uint8_t reserved_0[12];
	uint8_t drive_number;
	uint8_t reserved_1;
	uint8_t boot_signature;
	uint32_t volume_id;
	uint8_t volume_lable[11];
	uint8_t fat_type_label[8];
};
struct __fat_BS{
	unsigned char 		bootjmp[3];
	unsigned char 		oem_name[8];
	unsigned short 	        bytes_per_sector;
	unsigned char		sectors_per_cluster;
	unsigned short		reserved_sector_count;
	unsigned char		table_count;
	unsigned short		root_entry_count;
	unsigned short		total_sectors_16;
	unsigned char		media_type;
	unsigned short		table_size_16;
	unsigned short		sectors_per_track;
	unsigned short		head_side_count;
	unsigned int 		hidden_sector_count;
	unsigned int 		total_sectors_32;
 	struct __fat_extBS *extBS;
};
#endif
