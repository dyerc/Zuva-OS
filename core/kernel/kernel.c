/* 
 * kernel.c - The main architecture independant entry point of the
 * kernel. Called after the architecture has initialised.
 * 
 * 
 * This file is part of the Zuva Operating System. 
 * 
 * (C) Copyright Chris Dyer & Zuvium 2009 - 2010. Do Not Re-distribute. 
 * For internal viewing only.
 * 
 */

#include <types.h>
#include <kernel.h>
#include <printf.h>
#include <mm.h>
#include <time.h>
#include <console.h>
#include <multiboot.h>

extern multiboot_header_t *multiboot;


void kernel_init()
{
    console_init();
    
    asm volatile("sti");
    //time_init();
}

void kernel_shutdown()
{
}
