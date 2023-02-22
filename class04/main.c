#include "include/head.h"
#include <string.h>

struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;

void Main(void){
    int i;char s[256];

    init_gdtidt();
    init_pic();
    io_sti();


    init_palette();

    boxfill8(binfo->vram, binfo->scrnx, 0,0,0,binfo->scrnx, binfo->scrny);
    sprintf(s, "scrnx = %d", binfo->scrnx);
    putfonts8_asc(binfo->vram, binfo->scrnx, 8, 8, 7, s);
    io_set8(PIC0_IMR, 0xf9); /* 开放PIC1和键盘中断(11111001) */

    for (;;) {
        io_hlt();
    }
}