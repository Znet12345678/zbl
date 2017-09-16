/*
*minOS ata reader
*limited hardware support
*(c) 2015 Zachary James Schlotman
*/
#include <stdint.h>
#include "lib.h"
void delay(uint16_t io){
	for(int i = 0; i < 4;i++)
		inb(io + 0x0C);
}
uint8_t send_ident_cmd(uint16_t io,uint8_t slave){
	outb(io + 6,slave);
        outb(io + 2,1);
        outb(io + 3,0);
        outb(io + 4,0);
        outb(io + 5,0);
        outb(io + 7,0xEC);
        uint8_t status = inb(io + 7);
	uint8_t stat = inb(io + 7);
	if(status == 0)
		return status;
	while(stat & 0x80)
		stat = inb(io + 7);
	if(stat & 0x01){
		return 0;
	}
	stat = inb(io + 7);
	uint8_t val = inb(io + 4);
	//if(val != 0)
	//	return 0;
	//val = inb(io + 5);
	//if(val != 0)
	//	return 0;
	while(!(stat & 0x08)){
		if(stat & 1)
			return 0;
		stat = inb(io + 7);
	}
	for(int i = 0; i < 256;i++)
		inw(io);
	return stat;
}
int *detect_drives(){
	int *drives = malloc(1024);
	print("Trying primary master...\n");
	outb(0x1f6,0xa0);
	uint8_t status = send_ident_cmd(0x1F0,0xA0);
	if(status == 0)
		drives[0] = 0;
	else{
		print("Master is detected\n");
		drives[0] = 1;
	}
	print("Trying Primary slave\n");
	outb(0x1F6,0xb0);
	status = send_ident_cmd(0x1F0,0xB0);
	if(status == 0)
		drives[1] = 0;
	else{
		print("Slave is detected\n");
		drives[1] = 1;
	}
	print("Trying secondary master\n");
	outb(0x176,0xb0);
	status = send_ident_cmd(0x170,0xA0);
	if(status == 0)
		drives[2] = 0;
	else{
		print("Secondary master is detected\n");
		drives[2] = 1;
	}
	print("Trying secondary slave\n");
	outb(0x176,0xb0);
	status = send_ident_cmd(0x170,0xB0);
	if(status  == 0)
		drives[3] = 0;
	else{
		print("Secondary slave is detected\n");
		drives[3] = 1;
	}
	return drives;
}
/*static inline void outw(uint16_t port,uint8_t val){
     __asm__ volatile ("outw %w0, %w1" : : "a"(val), "Nd"(port));
}
static inline void outsw(uint16_t port,void *val,unsigned long n){
	unsigned long cnt = 1;
	__asm__ volatile ("rep outsw" :  "+S"(val),"+c"(n) : "d"(port));
}
static inline uint16_t inw(uint32_t s){
        uint16_t ret;
        __asm__ volatile("inw %%dx, %%ax": "=a"(ret):  "d"(s));
        return ret;
}*/
int ide_ident(int io){
	int drive=0x00;
	outb(io + 6,0xA0);
	outb(io + 2,0);
	outb(io + 3,0);
	outb(io + 4,0);
	outb(io + 5,0);
	outb(io + 7,0xEC);
}
int ide_wait_for_write(){
    for(int i = 0; i < 4;i++){
        inb(0x1FC);
    }
    uint8_t stat = inb(0x1F7);
    while(stat & 0x80)
        stat = inb(0x1f7);
    stat = inb(0x1F7);
        if(!(stat & 0x08))
            return -1;

    return 0;
}
int ide_wait_for_read(uint16_t io){
	a:;int i = 0;
	while(i < 4){
		inb(io + 0x0C);
		i++;
	}
	uint8_t status;
	while((status = inb(io + 0x07))&0x80);
	status = inb(io + 0x07);
	if(status & 0x01) goto a;
	if(status & 0x20) goto a;
	if(!(status & 0x08)) goto a;
	return 1;
	/*
	retry:;
		uint8_t status = inportb(io + 0x07);
		//print("!");
		if(status & 0x80) goto retry;
	retry2: status = inb(io + 0x07);
		//print("+");
		if(status & 0x01){
			//sleep(10000);
			return -1;
			//panic();
		}
		if(!(status & 0x08)) goto retry2;
		return 1;*/
}
uint16_t get_io_addr(){
	print("Testing HDA\n");
	ide_ident(0x1f0);
	if(ide_wait_for_read(0x1f0) > 0){
		print("HDA works!\n");
		return 0x1f0;
	}
	print("Testing HDB\n");
	ide_ident(0x170);
	if(ide_wait_for_read(0x170) > 0){
		print("HDB works!\n");
		return 0x170;
	}
	print("Testing Floppy Drive\n");
	ide_ident(0x3f0);
	if(ide_wait_for_read(0x3f0) > 0){
		print("Floppy works\n");
		return 0x3f0;
	}
	print("Error determaning io address!\n");
	panic();
}
int ide_wait_for_read_silent(){
    int i = 0;
    while(i < 4){
        inb(0x1FC);
        i++;
    }
retry:;
    uint8_t status = inb(0x1F7);
    if(status & 0x80) goto retry;
retry2:status = inb(0x1F7);
    if(status & 0x01)
        return -1;
    if(!(status & 0x08)) goto retry2;
    return 1;
    
}
int _ide_wait_for_read(int io,int drive){
        int i = 0;
        while(i < 4){
                inb(io + 0x0C);
                i++;
        }
        retry:;
                uint8_t status = inportb(io + 0x07);
                print("Drive is busy\n");
		//return -1;
                if(status & 0x80) goto retry2;
        retry2: status = inportb(io + 0x07);
                if(status & 0x01){
                        //print("I/O Error\n");
                        //sleep(10000);
                        return -1;
                        //panic();
                }
                if(!(status & 0x08)) goto retry2;
	        return 1;
}


unsigned int ddrive(){
	

}
int ata_read_master_nb(uint8_t *buf,uint32_t lba,uint8_t n){
	outb(0x1f6,(0xe0 | (uint8_t)((lba >> 24 & 0x0f))));
	outb(0x1f1,0);
	outb(0x1f2,n);
	outb(0x1f3,(uint8_t)lba);
	outb(0x1f4,(uint8_t)(lba >> 8));
	outb(0x1f5,(uint8_t)(lba >> 16));
	outb(0x1f7,0x20);
	if(ide_wait_for_read(0x1f0) < 0)
		panic();
	int i = 0;
	while(i < (n*512)){
		uint16_t dat = inw(0x1f0);
		*(uint16_t*)(buf + i * 2) = dat;
		i++;
	}
}
int __ata_read_master(uint8_t *buf,uint32_t lba,uint16_t drive){
	uint16_t io;
	uint8_t slavebit;
	if(drive == 0){
		io = 0x1f0;
		slavebit = 0xE0;
	}else if(drive == 1){
		io = 0x1f0;
		slavebit = 0xF0;
	}else if(drive == 2){
		io = 0x170;
		slavebit = 0xe0;
	}else if(drive == 3){
		io = 0x170;
		slavebit = 0xe0;
	}
	outb(io + 0x06,(slavebit | (uint8_t)((lba >> 24 & 0x0F))));
        outb(io + 1,0x00);
        outb(io + 0x02,1);
        outb(io + 0x03,(uint8_t)lba);
        outb(io + 0x04,(uint8_t)((lba) >> 8));
        outb(io + 0x05,(uint8_t)((lba) >> 16));
        outb(io + 0x07,0x20);
	if(ide_wait_for_read(io) < 0)
		return 0;
	int i = 0;
	while(i < 256){
                uint16_t data = inw(io);
                *(uint16_t *)(buf + i * 2) = data;
                i++;
        }
	return 1;
}
int ata_read_master(uint8_t *buf,uint32_t lba,uint16_t drive){
	uint16_t io;

	uint8_t cmd = 0xe0;
	if(*(int*)0x499 == 0){
		#ifdef DEBUG
		print("[ata]Using primary drive\n");
		#endif
		io = 0x1F0;
		cmd = 0xE0;
	}
	else if(*(int*)0x499 == 1){
		#ifdef DEBUG
		print("[ata]Using secondary drive\n");
		#endif
		io = 0x1F0;
		cmd = 0xF0;
	}
	else if(*(int*)0x499 == 2){
		io = 0x170;
		cmd = 0xE0;
	}
	else if(*(int*)0x499 == 3){
		io = 0x170;
		cmd = 0xF0;
	}
	uint8_t slavebit = 0x00;
	//print("Sending LBA and CMD\n");
	outb(io + 0x06,(cmd | (uint8_t)((lba >> 24 & 0x0F))));
	outb(io + 1,0x00);
	outb(io + 0x02,1);
	outb(io + 0x03,(uint8_t)lba);
	outb(io + 0x04,(uint8_t)((lba) >> 8));
	outb(io + 0x05,(uint8_t)((lba) >> 16));
	outb(io + 0x07,0x20);
	//ide_wait_for_read();
	//ide_wait_for_read(io);
	if(ide_wait_for_read(io) < 0)
		return 0;
	int i = 0;
	//print("Reading\n");
	while(i < 256){
		uint16_t data = inw(io);
		*(uint16_t *)(buf + i * 2) = data;
		i++;
	}
	delay(io);
	return 1;
}
int _ata_read_master(uint8_t *pntr,uint32_t lba,int drive){
	return ata_read_master(pntr,lba+*(int*)0x501,drive);
}
int ata_read_master_o(uint8_t *pntr,uint32_t lba,int offset){
	uint16_t io = 0x1f0;
	uint8_t cmd = 0xE0;
        uint8_t slavebit = 0x00;
        outb(io + 0x06,(cmd | (uint8_t)((lba >> 24 & 0x0F))));
        outb(io + 1,0x00);
        outb(io + 0x02,1);
        outb(io + 0x03,(uint8_t)lba);
        outb(io + 0x04,(uint8_t)((lba) >> 8));
        outb(io + 0x05,(uint8_t)((lba) >> 16));
        outb(io + 0x07,0x20);
	if(ide_wait_for_read(io) < 0)
		panic();
	int i = 0,j = 0;
	while(i < offset){
		inw(0x1f0);
		i++;
	}
	while(i < 512){
		uint16_t dat = inw(0x1f0);
		*(uint16_t*)(pntr + j * 2) = dat;
		i++;
		j++;
	}
}
int read_elf_on_disk(uint8_t *pntr,uint32_t lba,uint16_t io){
        uint8_t cmd = 0xE0;
        uint8_t slavebit = 0x00;
        outb(io + 0x06,(cmd | (uint8_t)((lba >> 24 & 0x0F))));
        outb(io + 1,0x00);
        outb(io + 0x02,108);
        outb(io + 0x03,(uint8_t)lba);
        outb(io + 0x04,(uint8_t)((lba) >> 8));
        outb(io + 0x05,(uint8_t)((lba) >> 16));
        outb(io + 0x07,0x20);
        if(ide_wait_for_read(io) < 0)
                return -1;
        int i = 0;
	while(i < (108 * 512)){
		uint16_t data = inw(io);
		*(uint16_t *)(pntr + i * 2) = data;
		i++;
	}
}
int ata_read_master_no(uint8_t *buf,uint32_t lba,uint8_t offset,uint8_t n,uint8_t e_offset){
	outb(0x1F6,(0xE0 | (uint8_t)((lba >> 24 & 0x0F))));
	outb(0x1F1,0);
	//if(n < 512)
	//	outb(0x1F2,1);
	//else if((n % 512) != 0)
	//	outb(0x1F2,n/512+1);
	//else
	//	outb(0x1F2,n/512);
	if(e_offset == 0)
		outb(0x1F2,n);
	else
		outb(0x1f2,n+1);
	outb(0x1F3,(uint8_t) lba);
	outb(0x1F4,(uint8_t) (lba >> 8));
	outb(0x1F5,(uint8_t) (lba >> 16));
	outb(0x1F7,0x20);
	if(ide_wait_for_read(0x1F0) < 0)
		panic();
	int i = 0;
	while(i < offset){
		inw(0x1F0);
		i++;
	}
	while(i < (n+1)*512){
		uint16_t val = inw(0x1F0);
		*(uint16_t*)(buf + i*2) = val;
		i++;
	}
	i = 0;
	while(i < e_offset){
		uint16_t val = inw(0x1f0);
		*(uint16_t*)(buf + i*2 + n*512) = val;
		i++;
	}
	return 0;
}
int ata_read_master_n(uint8_t *buf,uint32_t lba,unsigned int n){
        /*outb(0x1F6,(0xE0 | (uint8_t)((lba >> 24 & 0x0F))));
        outb(0x1F1,0);
        if(n < 512)
                outb(0x1F2,1);
        else
                outb(0x1F2,n/512);
        outb(0x1F3,(uint8_t) lba);
        outb(0x1F4,(uint8_t) (lba >> 8));
        outb(0x1F5,(uint8_t) (lba >> 16));
        outb(0x1F7,0x20);*/
	int io = 0x1F0;
        outb(io + 0x06,(0xE0 | (uint8_t)((lba >> 24 & 0x0F))));
        outb(io + 1,0x00);
	outb(io + 2,1);
        outb(io + 0x03,(uint8_t)lba);
        outb(io + 0x04,(uint8_t)((lba) >> 8));
        outb(io + 0x05,(uint8_t)((lba) >> 16));
        outb(io + 0x07,0x20);

        if(ide_wait_for_read(0x1F0) < 0)
                panic();
        int i = 0;
	while(i < n){
		uint16_t val = inw(0x1F0);
		//print("%c",val);
		*(uint16_t *)(buf + i * 2) = val;
		//print("%d",val);
		i++;
	}
	return 0;
}
int ata_write_master_no(uint8_t *buf,uint16_t lba,unsigned int offset,unsigned int n){
	int i = 0;
	outb(0x1F6,(0xe0 | (uint8_t)(lba >> 24 & 0x0f)));
	if(n < 512)
		outb(0x1F2,1);
	else
		outb(0x1F2,n/512);
	outb(0x1F3,(uint8_t)lba);
	outb(0x1F4,(uint8_t)(lba >> 8));
	outb(0x1F5,(uint8_t)(lba >> 16));
	outb(0x1F7,0x30);
	if(ide_wait_for_write() < 0){
		print("I/O Error\n");
		panic();
	}
	char *__buf = blmalloc(1024);
	ata_read_master(__buf,lba,0);
	if((offset % 2) == 0)
		outsw(0x1F0,__buf,offset/2);
	else
		outsw(0x1F0,__buf,offset/2 + 1);
	if((n%2) == 0){
		outsw(0x1F0,buf,n/2);
		char _buf[513] = {[0 ... 512]0};
		outsw(0x1F0,_buf,256 - n/2);
        }
	else{
		outsw(0x1F0,buf,n/2+1);
                char _buf[513] = {[0 ... 512]0};
                outsw(0x1F0,_buf,256 - n/2+1);
	}
	return 0;
}
int ata_write_master_n(uint8_t *buf,uint16_t lba,unsigned int n){
    int i = 0;
    uint16_t io = 0x1F0;
    uint8_t cmd = 0x30;
    uint8_t slavebit = 0;
    outb(0x1F6,(0xE0 | (uint8_t)((lba >> 24 & 0x0F))));
    outb(0x1F1,0);
    outb(0x1F2,1);
    outb(0x1F3,(uint8_t)lba);
    outb(0x1F4,(uint8_t)(lba >> 8));
    outb(0x1F5,(uint8_t)(lba >> 16));
    outb(0x1F7,0x30);
    /*if(ide_wait_for_write() < 0){
	print("I/O Error!\n");
	panic();
    }*/
    int j = 0;
    if((n % 2) == 0){
    	outsw(0x1F0,buf,n/2);
    	char _buf[513] = {[0 ... 512]0};
    	outsw(0x1F0,_buf,256 - n/2);
    }else{
	outsw(0x1F0,buf,n/2 + 1);
        char _buf[513] = {[0 ... 512]0};
        outsw(0x1F0,_buf,256 - n/2 + 1);

    }
    return 0;

}
int ata_write_master(uint8_t *buf,uint32_t _lba){
    uint16_t io;
    uint32_t lba = _lba;
    io = 0x1F0;
    uint8_t cmd = 0x30;
    uint8_t slavebit = 0x00;
    outb(io + 0x06,(0xE0 | (uint8_t)((lba >> 24 & 0x0F))));
    outb(io + 0x02,1);
    outb(io + 0x03,(uint8_t)lba);
    outb(io + 0x04,(uint8_t)((lba) >> 8));
    outb(io + 0x05,(uint8_t)((lba) >> 16));
    outb(io + 0x07,0x30);
    if(ide_wait_for_write() < 0){
	print("I/O Error!\n");
	panic();
    }
    int i = 0;
   // kprintf("Writing\n");
    outsw(0x1F0,buf,512);
   // kprintf("Done\n");
}
int _ata_write_master(uint8_t *buf,uint32_t lba){
	return ata_write_master(buf,lba+*(int*)0x501);
}
int ata_write_master_no_no_ow(uint8_t *buf,uint16_t _lba,unsigned int offset,unsigned int n){
    //print("[b]LBA %d Offset:%d Number of bytes %d\n",_lba,offset,n);
    uint8_t _buf[1024];
   // char *_buf = blmalloc(1024);
    int lba = _lba;
    uint8_t __buf[1024];
    ata_read_master(_buf,lba,0);
    ata_read_master_no(__buf,lba,(offset + n),1,(offset - n));
    //print("%s\n",_buf);
    //print("[a]LBA %d Offset:%d Number of bytes %d\n",_lba,offset,n);

/*	    uint16_t io;
    uint32_t lba = _lba;
    io = 0x1F0;
    uint8_t cmd = 0x30;
    uint8_t slavebit = 0x00;
    outb(io + 0x06,(0xE0 | (uint8_t)((lba >> 24 & 0x0F))));
    outb(io + 0x01,0);
    outb(io + 0x02,1);
    outb(io + 0x03,(uint8_t)lba);
    outb(io + 0x04,(uint8_t)((lba) >> 8));
    outb(io + 0x05,(uint8_t)((lba) >> 16));
    outb(io + 0x07,0x30);
    if(ide_wait_for_write() < 0){
	print("I/O Error\n");
	panic();
    }*/
   // for(int j = 0; j < n;j++)
//	print("%c",buf[j]);
    int i = 0;
    uint16_t io = 0x1F0;
    uint8_t cmd = 0x30;
    uint8_t slavebit = 0;
    outb(0x1F6,(0xE0 | (uint8_t)((_lba >> 24 & 0x0F))));
    outb(0x1F1,0);
    outb(0x1F2,1);
    outb(0x1F3,(uint8_t)_lba);
    outb(0x1F4,(uint8_t)(_lba >> 8));
    outb(0x1F5,(uint8_t)(_lba >> 16));
    outb(0x1F7,0x30);
    if(ide_wait_for_write() < 0){
        print("I/O Error!\n");
        panic();
    }
    i = 0;
   
    if((offset % 2) == 0)
    	outsw(0x1F0,_buf,offset/2);
    else
	outsw(0x1F0,_buf,offset/2+1);
    //print("Writing %s\n",buf);
  //  for(int j = 0; j < n;j++){
//	print("%d",buf[j]);
    //}
    if((n % 2) == 0){
   	 outsw(0x1F0,buf,n/2);
   	 outsw(0x1F0,__buf,(256 - (n/2 + offset)));
    }
    else{
	outsw(0x1F0,buf,n/2+1);
	outsw(0x1F0,__buf,(256 - (n/2 + offset) + 1));
    }
    //for(int i = 0; i < n;i++)
//	    outsw(0x1F0,buf,1);
//    for(int i = 0; i < (n/512) + 1;i++)
//	outsw(0x1F0,buf,512);
    //for(int i = 0; i < n;i++)
//	print("%c",buf[i]);
    return 0;
}
int ata_check_lba(uint32_t lba){
    uint16_t io;
    io = 0x1F0;
    uint8_t cmd = 0xE0;
    uint8_t slavebit = 0x00;
    outb(io + 0x06,(cmd | (uint8_t)((lba >> 24 & 0x0F))));
    outb(io + 1,0x00);
    outb(io + 0x02,1);
    outb(io + 0x03,(uint8_t)lba);
    outb(io + 0x04,(uint8_t)((lba) >> 8));
    outb(io + 0x05,(uint8_t)((lba) >> 16));
    outb(io + 0x07,0x20);
    uint16_t status;
    status = inb(0x1F7);
    if(status & 0x01)
        return -1;
    int i = 0;
    while(i < 256){
        uint16_t val = inw(0x1F0);
        i++;
    }
    return 0;
}
int zero_drive(){
    int i = 0;

    while(1){
        outb(0x1F0,0);
        outb(0x1F1,0);
        outb(0x1F2,0);
        outb(0x1F6,0);
        outb(0x1F7,0);
        //t_displaylq();
        uint16_t io = 0x1F0;
        uint8_t cmd = 0x30;
        uint8_t slavebit = 0;
        outb(0x1F0 + 0x06,(0xE0 | (uint8_t)(((i >> 24) & 0x0F))));
        outb(0x1F1,0);
        outb(0x1F2,1);

        outb(0x1F3,(uint8_t)i);
        outb(0x1F4,(uint8_t)(i >> 8));
        outb(0x1F5,(uint8_t)(i >> 16));
        outb(0x1F7,0x30);
        int j = 0;
        uint16_t val = 0;
        while(j < 512){
            outw(0x1F0,0);
           // outw(0x1F7,0xE7);
            j++;
        }
        i++;
    }
    return 0;
}
int zero_part(int partnum){
    char *buf = blmalloc(512);
    int start;
    int end;
    ata_read_master(buf,0,0x00);
    if(partnum == 0)
        return zero_drive();
    if(partnum == 1){
        start = buf[454] << 24 | buf[455] << 16 | buf[456] << 8 | buf[457];
        end = start + (buf[458] << 24 | buf[459] << 16 | buf[460] << 8 | buf[461]);
    }
    else if(partnum == 2){
        start = buf[470] << 24 | buf[471] << 16 | buf[472] << 8 | buf[473];
        end = start + (buf[474] << 24 | buf[475] << 16 | buf[476] << 8 | buf[477]);
    }
    else if(partnum == 3){
        start = buf[486] << 24 | buf[487] << 16 | buf[488] << 8 | buf[489];
        end = start + (buf[490] << 24 | buf[491] << 16 | buf[492] << 8 | buf[493]);
    }
    else if(partnum == 4){
        start = buf[502] << 24  | buf[503] << 16 | buf[504] << 8 | buf[505];
        end = start + (buf[506] << 24 | buf[507] << 16 | buf[508] << 8 | buf[509]);
    }

    int io = 0x1F0;
    for(int i = 0; i < (end - start);i++){
        uint8_t cmd = 0x30;
        uint8_t slavebit = 0x00;
        outb(0x1F6,0x30);
        outb(io + 0x06,(cmd | (uint8_t)((i >> 24 & 0x0F))));
        outb(io + 1,0x00);
        outb(io + 0x02,1);
        outb(io + 0x03,(uint8_t)i);
        outb(io + 0x04,(uint8_t)((i) >> 8));
        outb(io + 0x05,(uint8_t)((i) >> 16));
        if(ide_wait_for_read(io) < 0)
            return -1;
        int i = 0;
        while(i < 512){
            outb(io,0);
        }
    }
}
struct partition{
    int boot;
    int starting_head;
    int starting_sector;
    int starting_cylinder;
    int id;
    int ending_head;
    int ending_sector;
    int ending_cylinder;
    uint32_t relative_sector;
    uint32_t tsp;
};
int ata_read_part(uint8_t *rbuf,uint16_t lba,int partnum){
    char *buf = blmalloc(513);
    int start;
    int end;
    ata_read_master(buf,0,0x00);
    if(partnum == 0)
        return ata_read_master(rbuf,lba,0);
    if(partnum == 1){
        start = buf[454] << 24 | buf[455] << 16 | buf[456] << 8 | buf[457];
        end = start + (buf[458] << 24 | buf[459] << 16 | buf[460] << 8 | buf[461]);
    }
    else if(partnum == 2){
        start = buf[470] << 24 | buf[471] << 16 | buf[472] << 8 | buf[473];
        end = start + (buf[474] << 24 | buf[475] << 16 | buf[476] << 8 | buf[477]);
    }
    else if(partnum == 3){
        start = buf[486] << 24 | buf[487] << 16 | buf[488] << 8 | buf[489];
        end = start + (buf[490] << 24 | buf[491] << 16 | buf[492] << 8 | buf[493]);
    }
    else if(partnum == 4){
        start = buf[502] << 24  | buf[503] << 16 | buf[504] << 8 | buf[505];
        end = start + (buf[506] << 24 | buf[507] << 16 | buf[508] << 8 | buf[509]);
    }
    if((start + lba) > end)
        return -1;
    return ata_read_master(rbuf,lba + start,0);
}
int ata_write_part(uint8_t *rbuf,uint16_t lba,int partnum){
    char *buf = blmalloc(513);
    int start;
    int end;
    ata_read_master(buf,0,0x00);
    if(partnum == 0)
        return ata_read_master(rbuf,lba,0);
    if(partnum == 1){
        start = buf[454] << 24 | buf[455] << 16 | buf[456] << 8 | buf[457];
        end = start + (buf[458] << 24 | buf[459] << 16 | buf[460] << 8 | buf[461]);
    }
    else if(partnum == 2){
        start = buf[470] << 24 | buf[471] << 16 | buf[472] << 8 | buf[473];
        end = start + (buf[474] << 24 | buf[475] << 16 | buf[476] << 8 | buf[477]);
    }
    else if(partnum == 3){
        start = buf[486] << 24 | buf[487] << 16 | buf[488] << 8 | buf[489];
        end = start + (buf[490] << 24 | buf[491] << 16 | buf[492] << 8 | buf[493]);
    }
    else if(partnum == 4){
        start = buf[502] << 24  | buf[503] << 16 | buf[504] << 8 | buf[505];
        end = start + (buf[506] << 24 | buf[507] << 16 | buf[508] << 8 | buf[509]);
    }
    if((start + lba) > end)
        return -1;
    return ata_write_master(rbuf,lba + start);
}
int ata_read_cnt(uint8_t *buf,uint32_t lba,int cnt, uint16_t drive){
	int io;
	if(drive == 0x00)
		io = 0x01F0;
	else if(drive = 0x01)
		io = 0x170;
	uint8_t cmd = 0xE0;
	uint8_t slavebit = 0x00;
	outb(io + 0x06,(cmd | (uint8_t)((lba >> 24 & 0x0F))));
	outb(io + 1,0x00);
	outb(0x1F2,1);
	outb(0x1F3,(uint8_t)lba);
	outb(0x1F4,(uint8_t)((lba) >> 8));
	outb(0x1F5,(uint8_t)((lba) >> 16));
	outb(0x1F7,0x20);
	if(ide_wait_for_read(io) != 1)
		panic();
	int i = 0;
	while(i < 256){
		uint16_t data = inw(io);
		*(uint16_t *)(buf + i * 2) = data;
		i++;
	}
}
