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

/* naskfunc.nas */
void start_app(int eip, int cs, int esp, int ds);
void asm_api();
void farcall(int eip, int cs);
void farjmp(int eip, int cs);
void asm_inthandler20();
void io_stihlt();
void asm_cons_putchar(void);
void io_hlt(void);
void io_cli(void);
void io_sti(void);
int io_get8(int port);
void io_set8(int port, int data);
void store_cr0(int cr0);
int load_cr0(void);
unsigned int memtest_sub(unsigned int start, unsigned int end);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
void asm_inthandler20(void);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);

/* graphic.c */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen8(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
int pysize, int px0, int py0, char *buf, int bxsize);
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define BACKCOLOR		14
#define COL8_848484		15
#define FONTCOLOR		16
#define BARCOLOR		17

/* dsctbl.c */
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e

/* pic.c */
void init_pic(void);
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1
// input.c




//memory.c
#define MEMMAN_ADDR 0x003c0000
#define EFLAGS_AC_BIT 0x00040000
#define CR0_CACHE_DISABLE 0x60000000

#define MEMMAN_FREES 4090 /* 大约是32KB*/
struct FREEINFO { /* 可用信息 */
	unsigned int addr, size;
};
struct MEMMAN { /* 内存管理 */
	int frees, maxfrees, lostsize, losts;
	struct FREEINFO free[MEMMAN_FREES];
};
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memtest(unsigned int start, unsigned int end);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
void memory();
unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size);
int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size);

//timer
struct TIMERCTL {
	unsigned int count;
	unsigned int timer;
	unsigned int interval;
	unsigned int light;
	unsigned int reply;
	unsigned int due_time;
};

#define PIT_CTRL 0x0043
#define PIT_CNT0 0x0040
void init_timer(void);
void set_timer(int interval,int relpy);


/*Interrupt.c*/
#define PORT_KEYDAT 0x0060
void inthandler21(int *esp);
void inthandler27(int *esp);
void inthandler2c(int *esp);
void inthandler20(int *esp);

/*fifo.c*/
struct FIFO8 {
	unsigned char *buf;
	int p, q, size, free, flags;
};
#define FLAGS_OVERRUN	0x0001
void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf);
int fifo8_put(struct FIFO8 *fifo, unsigned char data);
int fifo8_get(struct FIFO8 *fifo);
int fifo8_status(struct FIFO8 *fifo);

/*keyboard.c*/
struct MOUSE_DEC {
	unsigned char buf[3], phase;
	int x, y, btn;
};
static char keytable[84] = {
0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0, 0,
'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '@', '[', 0, 0, 'a',
's','d', 'f', 'g', 'h', 'j', 'k', 'l', ';', ':', 0, 0, ']', 'z', 'x', 'c',
'v','b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+','1','2', '3', '0', '.'
};
#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4
#define PORT_KEYDAT 0x0060
#define PORT_KEYSTA 0x0064
#define PORT_KEYCMD 0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE 0x60
#define KBC_MODE 0x47
void enable_mouse(void);
void init_keyboard(void);
void wait_KBC_sendready(void);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);

/*array.c*/
void init_arr(unsigned char *arr);
void del_arr(unsigned char *arr);
void put_arr(unsigned char *arr,char data);
int arr_size(unsigned char *arr);

/*file.c*/
struct FILEINFO *file_search(char *name, struct FILEINFO *finfo, int max);
void file_readfat(int *fat, unsigned char *img);
void file_loadfile(int clustno, int size, char *buf, int *fat, char *img);

/*main.c*/
//static int line;



/*api.c*/
void api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax);
void print_char(int c);
void print_str(char *s);
/*console.c*/
#define ADR_DISKIMG 0x00100000
struct FILEINFO {
	unsigned char name[8], ext[3], type;
	char reserve[10];
	unsigned short time, date, clustno;
	unsigned int size;
};
int cmd_app(int *fat, char *cmdline);

