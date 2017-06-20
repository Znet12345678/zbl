#ifndef __FS_H
#define __FS_H
struct inf_sector{
	uint8_t username_len;
	uint8_t password_len;
	uint8_t *username;
	uint8_t *password;
};
#endif
