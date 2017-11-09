#include <lib.h>
#include <mem.h>
#include <env.h>

int shell(const char *cmd){
//	printf("{%s}\n",cmd);
	char **arr = sep(cmd,' ');
	int lst = 0;
	while(arr[lst] != 0)
		lst++;
	int argc = lst;
	char *fname = malloc(1024);
	char *pth = getenv("PATH");	
	char **ptharr = sep(pth,';');
	int pthlst = 0;
	while(ptharr[pthlst] != 0)
		pthlst++;
	for(int i = 0; i < pthlst;i++){
		strcpy(fname,"/fs/dos/");
		strcat(fname,ptharr[i] + 2);
		strcat(fname,"/");
		strcat(fname,cmd);
		strcat(fname,".exe");
		int fd = open(fname,O_RDONLY);
		uint8_t *buf = malloc(fsize(fname));
		int val = read(fd,buf,fsize(fname));
		if(!val)
			continue;
		int (*ret)(int argc,char *argv[]) = loadPE(buf);
		t_writevals();
		return ret(argc,arr);
	}
	kprintf("No such file or directory %s\n",cmd);
	return 0;
}
int main(int argc,char *argv[]){
	t_readvals();
	kprintf("Welcome to Protected mode DOS\n");
	setenv("PATH","C:/");
	setenv("CURRDIR","C:/");
	char *cmd = malloc(1024);
	while(1){
		for(int i = 0; i < 1024;i++)
			cmd[i] = 0;
		kprintf("\n%s>",getenv("CURRDIR"));
		gets(cmd);
		shell(cmd);
		t_readvals();
	}
}
