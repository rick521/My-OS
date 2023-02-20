#include "include/head.h"
#include <string.h>

struct TIMERCTL timerctl;
struct FIFO8 keyfifo;
struct FIFO8 mousefifo;
struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;

void inthandler21(int *esp)
{
	unsigned char data;
	io_set8(PIC0_OCW2, 0x61); /* 通知PIC，说IRQ-01的受理已经完成 */
	data = io_get8(PORT_KEYDAT);
	fifo8_put(&keyfifo, data);
	return;
}

void inthandler20(int *esp){
    io_set8(0x0020, 0x60);
	if(timerctl.due_time==++timerctl.count&&timerctl.timer==1){
		if(timerctl.reply==1){
			timerctl.due_time=timerctl.count+timerctl.interval;
			timerctl.light=~timerctl.light;// the light is on
		}else timerctl.light=~timerctl.light;// the light is on

	}
	if(timerctl.count%100==0){
		char s[30];
		boxfill8(binfo->vram, binfo->scrnx, BARCOLOR, 0, binfo->scrny-16,binfo->scrnx, binfo->scrnx);
		sprintf(s, "%d:%d:%d", timerctl.count/100,timerctl.interval,timerctl.light);
		putfonts8_asc(binfo->vram, binfo->scrnx, binfo->scrnx-strlen(s)*8, binfo->scrny-16, COL8_FFFFFF, s);
		//memory();
	}
	return;
}
void inthandler2c(int *esp)
/* 来自PS/2鼠标的中断 */
{
	unsigned char data;
	io_set8(PIC1_OCW2, 0x64); /* 通知PIC1 IRQ-12的受理已经完成 */
	io_set8(PIC0_OCW2, 0x62); /* 通知PIC0 IRQ-02的受理已经完成 */
	data = io_get8(PORT_KEYDAT);
	fifo8_put(&mousefifo, data);
	return;
}

void inthandler27(int *esp){
	io_set8(PIC0_OCW2, 0x67);
	return;
}

