#include "include/head.h"

int cmd_app(int *fat, char *cmdline)
{
    int segsiz, datsiz, esp, dathrb;
    extern int line;
    extern int times;
    struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
    struct FILEINFO *finfo;
    struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
    char name[18], *p,*q;
    int *esp0;
    int i;
    for (i = 0; i < 13; i++) {
        if (cmdline[i] <= ' ') {
            break;
        }
        name[i] = cmdline[i];
    }
    name[i] = 0;

    finfo = file_search(name, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
    if (finfo != 0) {
            p = (char *) memman_alloc_4k(memman, finfo->size);
            q = (char *) memman_alloc_4k(memman, segsiz); /*这里！*/
            file_loadfile(finfo->clustno, finfo->size, p, fat, (char *) (ADR_DISKIMG+ 0x003e00));
            segsiz = *((int *) (p + 0x0000));
            esp = *((int *) (p + 0x000c));
            datsiz = *((int *) (p + 0x0010));
            dathrb = *((int *) (p + 0x0014));
            p[0] = 0xe8;
            p[1] = 0x16;
            p[2] = 0x00;
            p[3] = 0x00;
            p[4] = 0x00;
            p[5] = 0xcb;
            for (i = 0; i < datsiz; i++) {
                q[esp + i] = p[dathrb + i];
            }
            *((int *) 0xfe8) = (int) q;
            set_segmdesc(gdt + 1003, finfo->size - 1, (int) p, AR_CODE32_ER);
            set_segmdesc(gdt + 1004,  segsiz - 1, (int) q, AR_DATA32_RW); /*这里！*/
           
            start_app(0, 1003 * 8, esp, 1004 * 8);
        memman_free_4k(memman, (int) q, segsiz);
        memman_free_4k(memman, (int) p, finfo->size);
        line++;
        times=0;
        return 1;
    }
    return 0;
}


