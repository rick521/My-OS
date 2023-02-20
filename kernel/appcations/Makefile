TOOLPATH = ../../z_tools/
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
RULEFILE = haribote.rul
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
DEL      = del

api.obj :
	$(NASK) api.asm api.obj

app.gas :app.c Makefile
	$(CC1) -o app.gas app.c

app.asm : app.gas Makefile
	$(GAS2NASK) app.gas app.asm

app.obj : app.asm Makefile
	$(NASK) app.asm app.obj

app.bim : api.obj app.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:app.bim stack:1k map:app.map api.obj app.obj

build :clean app.bim Makefile
	
	$(BIM2HRB) app.bim app.app 0


clean :
	-$(DEL) *.bim
	-$(DEL) *.map
	-$(DEL) *.gas
	-$(DEL) *.obj
	-$(DEL) app.asm