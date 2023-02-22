; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; 制作目标文件的模式	
[INSTRSET "i486p"]				; 使用到486为止的指令
[BITS 32]						; 3制作32位模式用的机器语言
[FILE "naskfunc.asm"]			; 文件名
	GLOBAL _io_hlt,_write_mem8,_io_cli,_io_sti,_io_get8,_io_set8,_io_stihlt
	GLOBAL _io_load_eflags,_io_store_eflags
		

[SECTION .text]

_io_hlt:	; void io_hlt(void);
		HLT
		RET

_io_cli:	; void io_cli(void);
		CLI
		RET

_io_sti:	; void io_sti(void);
		STI
		RET

_io_get8:	; int io_get8(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AL,DX
		RET

_io_set8:	; void io_set8(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		AL,[ESP+8]		; data
		OUT		DX,AL
		RET

_io_stihlt:	; void io_stihlt(void);
		STI
		HLT
		RET

_write_mem8: ; void write_mem8(int addr, int data);
		MOV ECX,[ESP+4] ; taking content of add
		MOV AL,[ESP+8] ; taking content of data
		MOV [ECX],AL ; *ecx=al
		RET
_io_load_eflags:	; int io_load_eflags(void);
		PUSHFD		; PUSH EFLAGS 
		POP		EAX
		RET

_io_store_eflags:	; void io_store_eflags(int eflags);
		MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD		; POP EFLAGS 
		RET