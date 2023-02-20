#include "include/head.h"

struct MOUSE_DEC mdec;
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
{
    if (mdec->phase == 0) {
    /* 等待鼠标的0xfa的阶段 */
    if (dat == 0xfa) {
        mdec->phase = 1;
    }
    return 0;
    }
    if (mdec->phase == 1) {
    /* 等待鼠标第一字节的阶段 */
        if ((dat & 0xc8) == 0x08) {
    /* 如果第一字节正确 */
            mdec->buf[0] = dat;
            mdec->phase = 2;
    }
    return 0;
    }
    if (mdec->phase == 2) {
    /* 等待鼠标第二字节的阶段 */
        mdec->buf[1] = dat;
        mdec->phase = 3;
        return 0;
    }
    if (mdec->phase == 3) {
    /* 等待鼠标第三字节的阶段 */
        mdec->buf[2] = dat;
        mdec->phase = 1;
        mdec->btn = mdec->buf[0] & 0x07;
        mdec->x = mdec->buf[1];
        mdec->y = mdec->buf[2];
    if ((mdec->buf[0] & 0x10) != 0) {
        mdec->x |= 0xffffff00;
    }
    if ((mdec->buf[0] & 0x20) != 0) {
        mdec->y |= 0xffffff00;
    }
    mdec->y = - mdec->y; /* 鼠标的y方向与画面符号相反 */
    return 1;
}
return -1; /* 应该不会到这儿来 */
}


void wait_KBC_sendready(void)
{
/* 等待键盘控制电路准备完毕 */
	for (;;) {
		if ((io_get8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
	return;
}
void init_keyboard(void)
{
/* 初始化键盘控制电路 */
	wait_KBC_sendready();
	io_set8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_set8(PORT_KEYDAT, KBC_MODE);
	return;
}
void enable_mouse(void)
{
	/* 激活鼠标 */
	wait_KBC_sendready();
	io_set8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_set8(PORT_KEYDAT, MOUSECMD_ENABLE);
	return; /* 顺利的话,键盘控制其会返送回ACK(0xfa)*/
}
