#include "include/head.h"
#include <string.h>
struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
extern struct FIFO8 keyfifo;
extern struct TIMERCTL timerctl;
struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;

void Main(void){
    int i;char s[256];
    char keybuf[256];

    init_gdtidt();
    init_pic();
    io_sti();
    fifo8_init(&keyfifo, 32, keybuf);

    init_palette();
    init_pit();

    io_set8(PIC0_IMR, 0xf8); /* 开放PIC1和键盘中断(11111001) */
 
    memory();
    for (;;) {
        io_hlt();
        boxfill8(binfo->vram, binfo->scrnx, 1,0,24,binfo->scrnx, 40);
        sprintf(s, "%010d", timerctl.count);
        putfonts8_asc(binfo->vram, binfo->scrnx, 24, 24, 7, s);
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
                if(i==28){
                    char *ss=memman_alloc(memman,300 * sizeof(char));
                    sprintf(ss,"pi=%s","");
                    putfonts8_asc(binfo->vram, binfo->scrnx, 8, 48, 8, ss);
                }
            }else{
                boxfill8(binfo->vram, binfo->scrnx, 0,0,0,binfo->scrnx, binfo->scrny);
                sprintf(s, "key = -");
                putfonts8_asc(binfo->vram, binfo->scrnx, 8, 8, 7, s);
            }
            
        }
    }
}