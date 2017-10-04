/*
*Kernel Header
*zBL C Library
*/
#ifndef __KERNEL_H/*__KERNEL_H*/
#define __KERNEL_H
/*
*Prints str to screen
*/
void print(const char *str);
/*
*puts c to screen
*/
void t_putc(char c);
/*
*Saves the terminal data
*/
void t_writevals();
/*
*Loads the terminal data
*/
void t_readvals();
extern char **env;
int fsize(const char *filename);
int *kexec(void *unused,void *pntr);
#endif/*__KERNEL_H*/
