#include "lib.h"
#include "fs.h"
#include "mem.h"
int has_sysinf(){
	return 0;
}
int end_of_prefs(){
	int lba = 51;
	a:;int pos = 0;
	uint8_t *buf = malloc(1024);
	int err = ata_read_master(buf,lba,0);
	if(err < 0)
		panic();
	c:while(buf[pos] != 0x7f && pos < 512)
		pos++;
	if(pos == 512){
		lba++;
		goto a;
	}
	int origpos = pos;
	if(buf[pos + 1] != 'z' || buf[pos + 2] != 'B' || buf[pos + 3] != 'L')
		goto c;
	pos+=4;
	int finlba = 0;
	while(1){
		if(buf[pos] != 1){
			kprintf("Something has gone wrong! Dump:%d %d %d\n",buf[pos],lba + finlba,pos);
			break;
		}
		int size = buf[pos + 1];
		int namelen = buf[pos + 2];
		int _origpos = pos;
		int nxt_lba = buf[namelen + pos + 13] << 24 | buf[namelen + pos + 14] << 16 | buf[namelen + pos + 15] << 8 | buf[namelen + pos + 16];
		pos = buf[namelen + pos + 17] << 8 | buf[namelen + pos + 18] + origpos;
		finlba = nxt_lba;
		if(buf[namelen + _origpos + 19] == 0x6f)
			break;
	//	finlba = nxt_lba;
		ata_read_master(buf,nxt_lba + lba,0);
		if((pos + 100) > 512){
			uint8_t *buf2 = malloc(512);
			ata_read_master(buf2,nxt_lba + lba + 1,0);
			for(int i = 1; i < 512;i++)
				buf[i + 512] = buf2[i];
		}
	}
	kprintf("End_lba:%d\n",finlba);
	return finlba + lba;
}
int gen_sysinf(){
	a:;kprintf("Enter a username >>>");
	char *username = malloc(1024);
	gets(username);
	kprintf("\nEnter a password >>>");
	char *password = malloc(1024);
	silent_gets(password);
	char *correct = malloc(80);
	kprintf("\nReenter password >>>");
	char *cmp = malloc(1024);
	silent_gets(cmp);
	if(strcmp(cmp,password) != 0){
		kprintf("\nPasswords dont match:%s and %s\n",password,cmp);
		goto a;
	}
	b:;kprintf("\nIs the username %s correct>",username);
	gets(correct);
	if(strcmp(correct,"no") == 0){
		kprintf("\n");
		goto a;
	}else if(strcmp(correct,"yes") == 0){

	}else{
		correct = malloc(1024);
		kprintf("\ninvalid response %s\n",correct);
		goto b;
	}
	struct inf_sector *inf = malloc(512);
	inf->username_len = strlen(username);
	inf->password_len = strlen(password);
	inf->username = malloc(inf->username_len);
	inf->password = malloc(inf->password_len);
	strcpy(inf->username,username);
	strcpy(inf->password,password);
	int lba = end_of_prefs();
}
