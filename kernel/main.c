/* bootpack */

#include "include/head.h"
#include <stdio.h>
#include <string.h>

extern struct FIFO8 keyfifo;
extern struct TIMERCTL timerctl;
extern struct FIFO8 mousefifo;
extern struct MOUSE_DEC mdec;
struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
//struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
int *fat;



int line=1;
int times;
void Main(void)
{
	struct FILEINFO *finfo = (struct FILEINFO *) (ADR_DISKIMG + 0x002600);

	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[50],mcursor[256],keybuf[32],mousebuf[256],cmdbuf[256],cmdrult[512];
	int len;
	char s2[50];
	int mx, my,i,j;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	
	init_gdtidt();
	init_pic();
	io_sti(); /* IDT/PIC的初始化已经完成，于是开放CPU的中断 */
	init_timer();
	set_timer(50,1);//1s
	init_palette();
	init_arr(cmdbuf);





	


	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
	mx = (binfo->scrnx - 16) / 2; /* 计算画面的中心坐标*/
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, BACKCOLOR);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	


	io_set8(PIC0_IMR, 0xf8); /* 开放PIC1和键盘中断(11111001) */
	io_set8(PIC1_IMR, 0xef); /* 开放鼠标中断(11101111) */
	init_keyboard();
	enable_mouse();
	
	
	fifo8_init(&keyfifo, 32, keybuf);
	fifo8_init(&mousefifo, 128, mousebuf);
	
	

	char head[5]={'r','o','o','t','#'};
	int p=0;
	for(p;p<=4;p++)
		put_arr(cmdbuf,head[p]);

	fat=(int *) memman_alloc_4k(memman, 4 * 2880);

	for (;;) {
		if(timerctl.light!=0)
			boxfill8(binfo->vram, binfo->scrnx, BACKCOLOR, 0, line*16,binfo->scrnx, line*16+2);
		else if(timerctl.timer==1)
			boxfill8(binfo->vram, binfo->scrnx, FONTCOLOR, arr_size(cmdbuf)*8, line*16,arr_size(cmdbuf)*8+8, line*16+2);
		
		io_cli();
		
		sprintf(s,"%s",cmdbuf);
		sprintf(s2, "%d:%d",i,arr_size(cmdbuf));

		boxfill8(binfo->vram, binfo->scrnx, BACKCOLOR, 0, (line-1)*16,binfo->scrnx, line*16);
		putfonts8_asc(binfo->vram, binfo->scrnx,0, (line-1)*16, FONTCOLOR, s);
		putfonts8_asc(binfo->vram, binfo->scrnx, 0, binfo->scrny-16, COL8_FFFFFF, s2);
		

		
		


		if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
			io_stihlt();
		} else {
			if (fifo8_status(&keyfifo) != 0) {
				i = fifo8_get(&keyfifo);
				io_sti();
				
				if(i<129)
				{

					put_arr(cmdbuf,keytable[i]);	
					
					if(i==28){
						boxfill8(binfo->vram, binfo->scrnx, BACKCOLOR, 0, line*16,binfo->scrnx, line*16+2);
						++line;
						char head[5]={'r','o','o','t','#'};
						char *cmd = cmdbuf + 5;
						
				
						sprintf(s,"%s",cmd);


						if(!strcmp(s,"clear")){
							putfonts8_asc(binfo->vram, binfo->scrnx, binfo->scrnx-300, binfo->scrny-16, COL8_FFFFFF, s);
							boxfill8(binfo->vram, binfo->scrnx, BACKCOLOR, 0, 0,binfo->scrnx, line*16);
							line=1;
						}else if (!strcmp(s,"htop")){
							struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
							unsigned int memtotal;
							struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
							memtotal = memtest(0x00400000, 0xbfffffff);
							memman_init(memman);
							memman_free(memman, 0x00001000, 0x0009e000); /* 0x00001000 - 0x0009efff */
							memman_free(memman, 0x00400000, memtotal - 0x00400000);
							sprintf(cmdrult, "memory %dKB free : %dKB",memtotal / (1024), memman_total(memman) / (1024));
							
							putfonts8_asc(binfo->vram, binfo->scrnx,0, (line++-1)*16, FONTCOLOR, cmdrult);
							init_arr(cmdrult);


						}else if (!strcmp(s,"ls")){
							int x,y;
							for (x = 0; x < 224; x++) {
								if (finfo[x].name[0] == 0x00) {
									break;
								}
								if (finfo[x].name[0] != 0xe5) {
									if ((finfo[x].type & 0x18) == 0) {
										sprintf(s, "filename.ext %7dKB",finfo[x].size/1024);
										for (y = 0; y < 8; y++) {
											s[y] = finfo[x].name[y];
										}
										s[ 9] = finfo[x].ext[0];
										s[10] = finfo[x].ext[1];
										s[11] = finfo[x].ext[2];
										char *cmdrult=s;
										putfonts8_asc(binfo->vram, binfo->scrnx,0, (line++-1)*16, FONTCOLOR, cmdrult);
										//init_arr(cmdrult);
									}
								}
							}
							init_arr(cmdrult);

							
						}else {
							char *cmd = cmdbuf + 5;

							if(!cmd_app(fat,cmd)){
								char *cmdrult="command not found";
								putfonts8_asc(binfo->vram, binfo->scrnx,0, (line++-1)*16, FONTCOLOR, cmdrult);
							}
						}
						
						
						
						
						
						
						init_arr(cmdbuf);
						int p=0;
						for(p;p<=4;p++)
							put_arr(cmdbuf,head[p]);
						//del_arr(cmdbuf);
					}else if(i==14){
						del_arr(cmdbuf);
					}
									
				}
			} else if (fifo8_status(&mousefifo) != 0) {
				j = fifo8_get(&mousefifo);
				io_sti();
				if (mouse_decode(&mdec, j) != 0) {
				/* 数据的3个字节都齐了，显示出来 */
					//sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
				if ((mdec.btn & 0x01) != 0) {
					//s[1] = 'L';

				}
				if ((mdec.btn & 0x02) != 0) {
					//s[3] = 'R';
				}
				if ((mdec.btn & 0x04) != 0) {
					//s[2] = 'C';
				}
				
				/* 鼠标指针的移动 */
				boxfill8(binfo->vram, binfo->scrnx, BACKCOLOR, mx, my, mx + 15, my +15); /* 隐藏鼠标 */
				mx += mdec.x;
				my += mdec.y;
				if (mx < 0) {
					mx = 0;
				}
				if (my < 0) {
					my = 0;
				}
				if (mx > binfo->scrnx - 16) {
					mx = binfo->scrnx - 16;
				}
				if (my > binfo->scrny - 16) {
					my = binfo->scrny - 16;
				}
					
					putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16); /* 描画鼠标 */
				}
			}
		}

	}
	

}



