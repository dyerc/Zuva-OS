#ifndef ARCH_X86_H
#define ARCH_X86_H

#include <types.h>
#include <time.h>

#define MAX_CPUS 16

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

typedef struct
{
    uint32_t ds;
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

typedef struct
{
    char *name;
    
} cpu_t;

void arch_cpu_init();

extern cpu_t cpu[MAX_CPUS];

void arch_init();
void arch_shutdown();

/************************ Descriptor Tables *****************************/

void arch_gdt_install();
void arch_idt_install();

typedef struct
{
   uint16_t limit;
   uint32_t base;     

} __attribute__((packed)) descriptor_ptr_t;

typedef struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
	
} __attribute__((packed)) gdt_entry_t;

extern void arch_gdt_reload();

typedef struct
{
    uint16_t base_lo;
    uint16_t sel;        /* Our kernel segment goes here! */
    uint8_t always0;     /* This will ALWAYS be set to 0! */
    uint8_t flags;       /* Set using the above table! */
    uint16_t base_hi;
    
} __attribute__((packed)) idt_entry_t;

extern void arch_idt_reload(uint32_t);

typedef void (*irq_t)(registers_t*);
extern irq_t irq_handlers[];

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void isr128();

void arch_register_irq_handler(uint8_t n, irq_t handler);

/************************    RTC    *****************************/

#define RTC_ADDRESS 0x70
#define RTC_DATA    0x71

time_detailed_t* arch_rtc_get_time();

/************************    PIT    *****************************/

void arch_pit_install();
void arch_pit_wait(uint32_t msec);

/************************   Paging   ****************************/

#include <mm.h>

void arch_pages_init(uint64_t start, uint64_t _memory_size);

void paging_frame_alloc(uint32_t address);
void paging_frame_free(uint32_t address);
void paging_frame_reserve(uint32_t address);
uint32_t paging_frame_status(uint32_t address);
//void paging_map_range(mm_context_t *context, uint32_t virtual_start, uint32_t virtual_end, uint32_t physical_address);
//void mm_switch_context(mm_context_t* new_context);
void paging_disable();

#endif
