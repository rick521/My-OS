#include "include/head.h"
#include <string.h>

struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
extern struct FIFO8 keyfifo;

void Main(void){
    int i;char s[256];
    char keybuf[256];

    init_gdtidt();
    init_pic();
    io_sti();
	fifo8_init(&keyfifo, 32, keybuf);

    init_palette();

    io_set8(PIC0_IMR, 0xf9); /* 开放PIC1和键盘中断(11111001) */
 
    memory();
    for (;;) {
        io_hlt();
        if (fifo8_status(&keyfifo)== 0) {
			io_stihlt();
		} else {
			int i = fifo8_get(&keyfifo);
			io_sti();	
			if(i<129)
			{
                boxfill8(binfo->vram, binfo->scrnx, 0,0,0,binfo->scrnx, binfo->scrny);
                sprintf(s, "key = %d", i);
                putfonts8_asc(binfo->vram, binfo->scrnx, 8, 8, 7, s);
            }else{
                boxfill8(binfo->vram, binfo->scrnx, 0,0,0,binfo->scrnx, binfo->scrny);
                sprintf(s, "key = -");
                putfonts8_asc(binfo->vram, binfo->scrnx, 8, 8, 7, s);
            }
            
        }
    }
}