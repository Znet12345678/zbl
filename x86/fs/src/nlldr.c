#ifdef OBSOLETE
#include <lib.h>
int main(int argc,char *argv[]){
	t_readvals();
	if(argc < 2)
		return 0;
        char *path = malloc(1024);
        strcpy(path,"/fs/exec/");
        strcat(path,argv[1]);
        strcat(path,".elf");
        int size = fsize(path);
        if(!size)
                return 0;
        uint8_t *buf = malloc(size);
        int ret = read_file(path,buf);
        if(!ret)
                return 0;
//	int fd = open(path,O_RDONLY,0);
//	read(fd,buf,size);
        uint8_t *pntr = (uint8_t*)0x8048000;
        uint8_t *jmp = exec_elf(pntr,buf);
        int (*func)(int argc,char **argv) = (int*)jmp;
        kprintf("\n");
        t_writevals();
        free(buf);
        free(path);
        return func(argc,argv);
}
#endif
