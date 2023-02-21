
#include "include/head.h"

struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;

void Main(void){
    int i;
    init_palette();
    boxfill8(binfo->vram, binfo->scrnx, 0,0,0,binfo->scrnx, binfo->scrny);
    for (;;) {
        io_hlt();
    }
}