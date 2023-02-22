.PHONY:appcations

TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/
ASMPATH = ./asm/
MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
OBJS_MAIN = main.obj naskfunc.obj graphic.obj font.obj interrupt.obj pic.obj dsctbl.obj
FONTPATH = ./font/
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe


BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

# 默认动作


%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

%.asm : %.gas Makefile
	$(GAS2NASK) $*.gas $*.asm

%.obj : %.asm Makefile
	$(NASK) $*.asm $*.obj



naskfunc.obj : $(ASMPATH)naskfunc.asm Makefile
	$(NASK) $(ASMPATH)naskfunc.asm naskfunc.obj



ipl10.bin : $(ASMPATH)ipl10.asm Makefile
	$(NASK) $(ASMPATH)ipl10.asm ipl10.bin

asmhead.bin : $(ASMPATH)asmhead.asm Makefile
	$(NASK) $(ASMPATH)asmhead.asm asmhead.bin

font.bin : $(FONTPATH)font.txt Makefile
	$(MAKEFONT) $(FONTPATH)font.txt font.bin

font.obj : font.bin Makefile
	$(BIN2OBJ) font.bin font.obj _font

main.bim : $(OBJS_MAIN) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:main.bim stack:3136k map:main.map \
		$(OBJS_MAIN)
# 3MB+64KB=3136KB

main.hrb : main.bim Makefile
	$(BIM2HRB) main.bim main.hrb 0

system.sys : asmhead.bin main.hrb Makefile
	copy /B asmhead.bin+main.hrb system.sys
















system.img : ipl10.bin system.sys Makefile
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:system.sys to:@: \
		imgout:system.img



# 其他指令

build :
	$(MAKE) system.img

run :
	$(MAKE) build
	$(COPY) system.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu
# $(MAKE) src_only

clean :
	-$(DEL) *.bin
	-$(DEL) *.gas
	-$(DEL) *.obj
	-$(DEL) graphic.asm
	-$(DEL) main.asm
	-$(DEL) main.map
	-$(DEL) main.bim
	-$(DEL) main.hrb
	-$(DEL) system.sys

src_only :
	$(MAKE) clean
	-$(DEL) system.img
