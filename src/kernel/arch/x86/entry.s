;
;  entry.s (x86) - The architecture initialisation point for the
;  x86 (i386) processor.
;
;
;  This file is part of the Zuva Operating System.
;
;  (C) 2013 Chris Dyer
;
;

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
; NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

bits 32                         ; All instructions should be 32-bit.

[GLOBAL mboot]
[EXTERN code]
[EXTERN bss]
[EXTERN end]

mboot:
  dd  MBOOT_HEADER_MAGIC

  dd  MBOOT_HEADER_FLAGS
  dd  MBOOT_CHECKSUM

  dd  mboot
  dd  code
  dd  bss
  dd  end
  dd  start

[GLOBAL start]
[EXTERN arch_init]

start:
  push ebx      ; Push a pointer to the multiboot info structure.

  mov ebp, 0    ; Initialise the base pointer to zero so we can terminate stack traces

  cli
  call arch_init
  jmp $

;
; GDT
;

[GLOBAL arch_gdt_flush]
[EXTERN gdt_ptr]

arch_gdt_flush:
  lgdt [gdt_ptr]        ; Load the GDT with our '_gp' which is a special pointer
  mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 0x08:arch_gdt_flush_complete   ; 0x08 is the offset to our code segment: Far jump!

arch_gdt_flush_complete:
  ret

;
; IDT
;

[GLOBAL arch_idt_load]
[EXTERN idtp]

arch_idt_load:
  lidt [idtp]
  ret

;
; ISR
;

%macro ISR_NOERR 1
  global _isr%1
  _isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERR 1
  global _isr%1
  _isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_NOERR 17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31
ISR_NOERR 127

[EXTERN arch_isr_handler]
isr_common_stub:
  pusha
  push ds
  push es
  push fs
  push gs
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov eax, esp
  push eax
  ; Call the C kernel fault handler
  mov eax, arch_isr_handler
  call eax
  pop eax
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8
  iret

;
; IRQ
;

%macro IRQ_ENTRY 2
  global _irq%1
  _irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

IRQ_ENTRY 0, 32
IRQ_ENTRY 1, 33
IRQ_ENTRY 2, 34
IRQ_ENTRY 3, 35
IRQ_ENTRY 4, 36
IRQ_ENTRY 5, 37
IRQ_ENTRY 6, 38
IRQ_ENTRY 7, 39
IRQ_ENTRY 8, 40
IRQ_ENTRY 9, 41
IRQ_ENTRY 10, 42
IRQ_ENTRY 11, 43
IRQ_ENTRY 12, 44
IRQ_ENTRY 13, 45
IRQ_ENTRY 14, 46
IRQ_ENTRY 15, 47

[EXTERN arch_irq_handler]
irq_common_stub:
  pusha
  push ds
  push es
  push fs
  push gs
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov eax, esp
  push eax
  ; Call the C kernel hardware interrupt handler
  mov eax, arch_irq_handler
  call eax
  pop eax
  pop gs
  pop fs
  pop es
  pop ds
  popa
  add esp, 8
  iret
