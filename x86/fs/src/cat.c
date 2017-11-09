#include <lib.h>
int main(int argc,char *argv[]){
	t_readvals();
	for(int i = 1; i < argc; i++){
		int size = fsize(argv[i]);
		if(!size)
			return 0;
		char *buf = malloc(size);
		int fd = open(argv[1],O_RDONLY);
		//kprintf("%d %d %d %d\n",k->pos_lba,k->pos_offset,k->fent->data_ent_lba,k->fent->data_ent_offset);
		if(fd < 0){
			kprintf("Failed to open file!\n");
			return 0;
		}
		read(fd,buf,size);
//		read_file(argv[1],buf);
		for(int i = 0; i < size;i++)
			t_putc(buf[i]);
	}
}
