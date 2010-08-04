#ifndef ARCH_X86EMU_H
#define ARCH_X86EMU_H

typedef struct
{
    long 	ebx;
    long 	ecx;
    long 	edx;
    long 	esi;
    long 	edi;
    long 	ebp;
    long 	eax;
    long 	__null_ds;
    long 	__null_es;
    long 	__null_fs;
    long 	__null_gs;
    long 	orig_eax;
    long 	eip;
    uint16_t 	cs, __csh;
    long 	eflags;
    long 	esp;
    uint16_t 	ss, __ssh;

    /*** These are part of the v86 interrupt stackframe: ***/
    uint16_t 	es, __esh;
    uint16_t 	ds, __dsh;
    uint16_t 	fs, __fsh;
    uint16_t 	gs, __gsh;
    
} x86emu_regs_t;

typedef struct
{
    
} x86emu_t;

#endif