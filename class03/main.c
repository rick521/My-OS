
#include "include/head.h"
#include <string.h>

struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;

void Main(void){
    int i;char s[256];
    init_palette();
    boxfill8(binfo->vram, binfo->scrnx, 0,0,0,binfo->scrnx, binfo->scrny);
    sprintf(s, "scrnx = %d", binfo->scrnx);
    putfonts8_asc(binfo->vram, binfo->scrnx, 8, 8, 7, s);

    for (;;) {
        io_hlt();
    }
}