;
;  i386.s (x86) - The architecture dependeny assembler thats not inline for
;  x86 (i386) processor.
;
;
;  This file is part of the Zuva Operating System. 
;  
;  (C) Copyright Chris Dyer & Zuvium 2009 - 2010. Do Not Re-distribute. 
;  For internal viewing only.
;  
;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;        GDT        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global arch_gdt_reload
extern gdt_ptr

arch_gdt_reload:
    lgdt [gdt_ptr]        ; Load the GDT with our '_gp' which is a special pointer
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:arch_gdt_reload_complete   ; 0x08 is the offset to our code segment: Far jump!

arch_gdt_reload_complete:
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;     Interrupts    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

global arch_idt_reload

arch_idt_reload:
    mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter. 
    lidt [eax]        ; Load the IDT pointer.
    ret
