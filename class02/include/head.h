/*naskfunc.asm*/
void io_stihlt();
void io_hlt(void);
void io_cli(void);
void io_sti(void);
int io_get8(int port);
void io_set8(int port, int data);
void write_mem8(int addr, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

/* asmhead.nas */
struct BOOTINFO { /* 0x0ff0-0x0fff */
	char cyls; /* 启动区读磁盘读到此为止 */
	char leds; /* 启动时键盘的LED的状态 */
	char vmode; /* 显卡模式为多少位彩色 */
	char reserve;
	short scrnx, scrny; /* 画面分辨率 */
	char *vram;
};
#define ADR_BOOTINFO 0x00000ff0

/*graphic.c*/
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

