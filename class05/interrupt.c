#include "include/head.h"

struct FIFO8 keyfifo;

void inthandler21(int *esp)
/* 来自PS/2键盘的中断 */
{
    unsigned char data;
	io_set8(PIC0_OCW2, 0x61); /* 通知PIC，说IRQ-01的受理已经完成 */
	data = io_get8(PORT_KEYDAT);
	fifo8_put(&keyfifo, data);
	return;
}