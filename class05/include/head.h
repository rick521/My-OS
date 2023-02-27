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

void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

void asm_inthandler21(void);

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
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);

/*font*/
extern char font[4096];

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
#define ADR_IDT            0x0026f800
#define LIMIT_IDT        0x000007ff
#define ADR_GDT            0x00270000
#define LIMIT_GDT        0x0000ffff
#define ADR_BOTPAK        0x00280000
#define LIMIT_BOTPAK    0x0007ffff
#define AR_DATA32_RW    0x4092
#define AR_CODE32_ER    0x409a
#define AR_INTGATE32    0x008e

/* pic.c */
void init_pic(void);
#define PIC0_ICW1        0x0020
#define PIC0_OCW2        0x0020
#define PIC0_IMR        0x0021
#define PIC0_ICW2        0x0021
#define PIC0_ICW3        0x0021
#define PIC0_ICW4        0x0021
#define PIC1_ICW1        0x00a0
#define PIC1_OCW2        0x00a0
#define PIC1_IMR        0x00a1
#define PIC1_ICW2        0x00a1
#define PIC1_ICW3        0x00a1
#define PIC1_ICW4        0x00a1

/*interrupt.c*/
#define PORT_KEYDAT 0x0060

void inthandler21(int *esp);

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