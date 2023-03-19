#include "include/head.h"

struct FIFO8 keyfifo;
extern struct TIMERCTL timerctl;

void inthandler21(int *esp)
/* 来自PS/2键盘的中断 */
{
    unsigned char data;
    io_set8(PIC0_OCW2, 0x61); /* 通知PIC，说IRQ-01的受理已经完成 */
    data = io_get8(PORT_KEYDAT);
    fifo8_put(&keyfifo, data);
    return;
}
void inthandler20(int *esp)
{
    io_set8(PIC0_OCW2, 0x60); /* 把IRQ-00信号接收完了的信息通知给PIC */
    /* 暂时什么也不做 */
    timerctl.count++;
    return;
}