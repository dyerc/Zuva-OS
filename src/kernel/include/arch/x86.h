#ifndef ARCH_X86_H
#define ARCH_X86_H

#include <types.h>

/************************      IO      *****************************/

inline uint8_t inb(uint16_t port);
inline void outb(uint16_t port, uint8_t data);
inline uint16_t inw( uint16_t port );
inline void outw(uint16_t port, uint16_t data);
inline uint32_t inl( uint16_t port );
inline void outl(uint16_t port, uint32_t data);
inline void inws( uint16_t* buffer, size_t count, uint16_t port );
inline void outws( const uint16_t* buffer, size_t count, uint16_t port );
inline void inls( uint32_t* buffer, size_t count, uint16_t port );
inline void outls( const uint32_t* buffer, size_t count, uint16_t port );

/************************      CPU     *****************************/

#define EFLAG_CF ( 1 << 0 )  /* Carry flag */
#define EFLAG_ZF ( 1 << 6 )  /* Zero flag */
#define EFLAG_SF ( 1 << 7 )  /* Sign flag */
#define EFLAG_IF ( 1 << 9 )  /* Interrupt flag */
#define EFLAG_ID ( 1 << 21 ) /* ID flag */

typedef struct registers
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef struct
{
    long ebx;
    long ecx;
    long edx;
    long esi;
    long edi;
    long ebp;
    long eax;
    long __null_ds;
    long __null_es;
    long __null_fs;
    long __null_gs;
    long orig_eax;
    long eip;
    uint16_t cs, __csh;
    long eflags;
    long esp;

} x86_regs_t;

/************************  X86 Components  *****************************/

#define IRQ_OFF     { asm volatile ("cli"); }
#define IRQ_RESET   { asm volatile ("sti"); }


extern void arch_gdt_install();
extern void arch_idt_install();
extern void arch_isr_install();
extern void arch_idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
extern void arch_irq_install();
extern void irq_ack();

#endif
