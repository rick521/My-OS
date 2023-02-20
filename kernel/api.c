#include "include/head.h"
extern int line,times;

void api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	int cs_base = *((int *) 0xfe8); /*这里！*/
	if (edx == 1) {
		print_char(eax);
	} else if (edx == 2) {
		print_str((char *) ebx + cs_base);


	}
	return;
}


void print_char(int c){
	
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
    char cmdrult[256];
	cmdrult[0]=c;
	cmdrult[1]=0;
	if(cmdrult[0]==0x0a)
	{line++; times=0;}
	else{
		putfonts8_asc(binfo->vram, binfo->scrnx,times*8, (line-1)*16, FONTCOLOR, cmdrult);
		times++;
	}
	
}
void print_str(char *s)
{
	for (; *s != 0; s++) {
		print_char(*s);
	}
	return;
}

