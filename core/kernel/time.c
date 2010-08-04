/* 
 * irq.c - The architecture independent time manager. Stores system
 * uptime as well as sleep commands etc.
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

#include <time.h>
#include <printf.h>

uint32_t sys_uptime = 0;
time_detailed_t *time;

void sleep(uint32_t msec)
{
    arch_pit_wait(msec);
}

time_detailed_t* time_retrieve_detailed()
{
    return time;
}

/* Called by PIT interrupt each second */
void time_change()
{
    sys_uptime++;
    time->sec++;
}

void time_init()
{
    time = arch_rtc_get_time();
    arch_pit_install();
    
    kprintf("Time is: %d:%d:%d", time->hour, time->min, time->sec);
}