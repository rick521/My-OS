#include "include/head.h"

struct TIMERCTL timerctl;

void init_pit(void)
{
    io_set8(PIT_CTRL, 0x34);
    io_set8(PIT_CNT0, 0x9c);
    io_set8(PIT_CNT0, 0x2e);
    timerctl.count = 0;
    
    return;
}