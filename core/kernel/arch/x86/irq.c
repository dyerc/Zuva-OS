/* 
 * irq.c - The architecture level part of the interrupts
 * system for x86. All handlers are diverted to the architecture
 * independent part.
 * 
 * 
 * This file is part of the Zuva Operating System. 
 * 
 * (C) Copyright Chris Dyer & Zuvium 2009 - 2010. Do Not Re-distribute. 
 * For internal viewing only.
 * 
 */

#ifdef ARCH_X86
    #include <arch/x86.h>
#endif

#include <irq.h>
#include <types.h>
#include <console.h>
#include <printf.h>

void arch_register_irq_handler(uint8_t n, irq_t handler)
{
    irq_handlers[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
void arch_isr_handler(registers_t regs)
{
    // This line is important. When the processor extends the 8-bit interrupt number
    // to a 32bit value, it sign-extends, not zero extends. So if the most significant
    // bit (0x80) is set, regs.int_no will be very large (about 0xffffff80).
    uint8_t int_no = regs.int_no & 0xFF;
    if (irq_handlers[int_no] != 0)
    {
        irq_t handler = irq_handlers[int_no];
        handler(&regs);
    }
    else
    {
	// Unhandled interrupt
	kprintf("Unhandled interrupt!");
        for(;;);
    }
}

// This gets called from our ASM interrupt handler stub.
void arch_irq_handler(registers_t regs)
{
    if (regs.int_no >= 40)
    {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);

    if (irq_handlers[regs.int_no] != 0)
    {
        irq_t handler = irq_handlers[regs.int_no];
        handler(&regs);
    }

}