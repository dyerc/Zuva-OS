/* 
 * kernel.c - The PIT chip driver for the x86 architecture.
 * 
 * 
 * This file is part of the Zuva Operating System. 
 * 
 * (C) Copyright Chris Dyer & Zuvium 2009 - 2010. Do Not Re-distribute. 
 * For internal viewing only.
 * 
 */

#include <arch/x86.h>
#include <types.h>
#include <printf.h>
#include <time.h>

uint32_t timer_ticks = 0;
uint32_t frequency = 50;

void arch_pit_wait(uint32_t msec)
{
    uint32_t new_ticks = 0;

    new_ticks = timer_ticks + msec;
    while(timer_ticks < new_ticks);
}

static void arch_pit_irq(registers_t *regs)
{    
    timer_ticks++;
    
    if (timer_ticks % frequency == 0)
    {
		time_change();
    }
}

void arch_pit_install()
{  
    arch_register_irq_handler(32, &arch_pit_irq);
    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
}
