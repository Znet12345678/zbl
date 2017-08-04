#include "lib.h"
#include "mem.h"
#include "dev.h"
void write_devs(uint8_t *ident,uint8_t type,uint16_t io,uint8_t slave){
	dev_t dev = malloc(sizeof(*dev));
	dev->alloc = 1;
	dev->type = type;
	dev->ident = malloc(4);
	memcpy(dev->ident,ident,sizeof(dev->ident));
	dev->io = io;
	dev->slave = slave;
	dev_t cycle = (dev_t)0x00A00000;
	while(cycle->alloc == 1)
		cycle+=sizeof(*cycle);
	memcpy(cycle,dev,sizeof(*dev));
}
void init_devs(){
	kprintf("GENDEV: ");
	int *hd = detect_drives();
	int *pntr = (int*)0x00A00000;
	dev_t d;
	for(int i = 0; i < 100 * sizeof(*d);i++)
		pntr[i] = 0;
	if(hd[0]){
		kprintf("hda ");
		write_devs("hda",0,0x1f0,0);
	}
	if(hd[1]){
		kprintf("hdb ");
		write_devs("hdb",0,0x1F0,1);
	}
	if(hd[2]){
		kprintf("hdc ");
		write_devs("hdc",0,0x170,0);
	}
	if(hd[3]){
		kprintf("hdd ");
		write_devs("hdd",0,0x170,1);
	}
	kprintf("kbd ");
	write_devs("kbd",1,0,0);
	kprintf("csl ");
	write_devs("csl",2,0,0);	
	kprintf("zro \n");
	write_devs("zro",3,0,0);
}
