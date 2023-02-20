#include "include/head.h"

extern struct TIMERCTL timerctl;
void init_timer(void)
{
	io_set8(PIT_CTRL, 0x34);
	io_set8(PIT_CNT0, 0x9c);
	io_set8(PIT_CNT0, 0x2e);
	timerctl.timer=0;
	timerctl.count=0;
	timerctl.light=0;
	timerctl.reply=0;
	timerctl.interval=0;

	return;
}

void set_timer(int interval,int reply){
	timerctl.timer=1;
	timerctl.interval=interval;
	timerctl.due_time=timerctl.count+interval;
	timerctl.reply=reply;
}





