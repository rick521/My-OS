; naskfunc
; TAB=4

[FORMAT "WCOFF"]                ; 制作目标文件的模式    
[INSTRSET "i486p"]                ; 使用到486为止的指令
[BITS 32]                        ; 3制作32位模式用的机器语言
[FILE "naskfunc.asm"]            ; 文件名
    GLOBAL _io_hlt,_write_mem8,_io_cli,_io_sti,_io_get8,_io_set8,_io_stihlt
    GLOBAL _io_load_eflags,_io_store_eflags,_asm_inthandler21
    GLOBAL _load_gdtr, _load_idtr,_load_cr0,_store_cr0
    GLOBAL _memtest_sub,_asm_inthandler20
    EXTERN _inthandler21,_inthandler20

[SECTION .text]

_io_hlt:    ; void io_hlt(void);
        HLT
        RET

_io_cli:    ; void io_cli(void);
        CLI
        RET

_io_sti:    ; void io_sti(void);
        STI
        RET

_io_get8:    ; int io_get8(int port);
        MOV        EDX,[ESP+4]        ; port
        MOV        EAX,0
        IN        AL,DX
        RET

_io_set8:    ; void io_set8(int port, int data);
        MOV        EDX,[ESP+4]        ; port
        MOV        AL,[ESP+8]        ; data
        OUT        DX,AL
        RET

_io_stihlt:    ; void io_stihlt(void);
        STI
        HLT
        RET

_write_mem8: ; void write_mem8(int addr, int data);
        MOV ECX,[ESP+4] ; taking content of add
        MOV AL,[ESP+8] ; taking content of data
        MOV [ECX],AL ; *ecx=al
        RET
_io_load_eflags:    ; int io_load_eflags(void);
        PUSHFD        ; PUSH EFLAGS 
        POP        EAX
        RET

_io_store_eflags:    ; void io_store_eflags(int eflags);
        MOV        EAX,[ESP+4]
        PUSH    EAX
        POPFD        ; POP EFLAGS 
        RET
_load_gdtr:        ; void load_gdtr(int limit, int addr);
        MOV        AX,[ESP+4]        ; limit
        MOV        [ESP+6],AX
        LGDT    [ESP+6]
        RET

_load_idtr:        ; void load_idtr(int limit, int addr);
        MOV        AX,[ESP+4]        ; limit
        MOV        [ESP+6],AX
        LIDT    [ESP+6]
        RET
_asm_inthandler21:
        PUSH    ES
        PUSH    DS
        PUSHAD
        MOV        EAX,ESP
        PUSH    EAX
        MOV        AX,SS
        MOV        DS,AX
        MOV        ES,AX
        CALL    _inthandler21
        POP        EAX
        POPAD
        POP        DS
        POP        ES
        IRETD
_memtest_sub: ; unsigned int memtest_sub(unsigned int start, unsigned int end)
        PUSH EDI ; （由于还要使用EBX, ESI, EDI）
        PUSH ESI
        PUSH EBX
        MOV ESI,0xaa55aa55 ; pat0 = 0xaa55aa55;
        MOV EDI,0x55aa55aa ; pat1 = 0x55aa55aa;
        MOV EAX,[ESP+12+4] ; i = start;
mts_loop:
        MOV EBX,EAX
        ADD EBX,0xffc ; p = i + 0xffc;
        MOV EDX,[EBX] ; old = *p;
        MOV [EBX],ESI ; *p = pat0;
        XOR DWORD [EBX],0xffffffff ; *p ^= 0xffffffff;
        CMP EDI,[EBX] ; if (*p != pat1) goto fin;
        JNE mts_fin
        XOR DWORD [EBX],0xffffffff ; *p ^= 0xffffffff;
        CMP ESI,[EBX] ; if (*p != pat0) goto fin;
        JNE mts_fin
        MOV [EBX],EDX ; *p = old;
        ADD EAX,0x1000 ; i += 0x1000;
        CMP EAX,[ESP+12+8] ; if (i <= end) goto mts_loop;
        JBE mts_loop
        POP EBX
        POP ESI
        POP EDI
        RET
mts_fin:
        MOV [EBX],EDX ; *p = old;
        POP EBX
        POP ESI
        POP EDI
        RET
_load_cr0:        ; int load_cr0(void);
        MOV        EAX,CR0
        RET
_store_cr0:        ; void store_cr0(int cr0);
        MOV        EAX,[ESP+4]
        MOV        CR0,EAX
        RET
_asm_inthandler20:
        PUSH ES
        PUSH DS
        PUSHAD
        MOV EAX,ESP
        PUSH EAX
        MOV AX,SS
        MOV DS,AX
        MOV ES,AX
        CALL _inthandler20
        POP EAX
        POPAD
        POP DS
        POP ES
        IRETD