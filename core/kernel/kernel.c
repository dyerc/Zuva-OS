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

extern multiboot_t *multiboot;

void kernel_detect_location(const char *args)
{
}

void kernel_init()
{
    console_init();
    //time_init();
    
    mm_init(multiboot);
    
    //kprintf("Heap = %d\nBlock = %d\nChunk = %d\n\n", sizeof(mm_heap_t), sizeof(mm_block_t), sizeof(mm_chunk_t));
    
    uint32_t a = kmalloc(1000);
    kprintf("A = %d\n", a);
    uint32_t b = kmalloc(100);
    kprintf("B = %d\n", b);
    kfree(b);
    uint32_t c = kmalloc(100);
    kprintf("C = %d\n", c);
}

void kernel_shutdown()
{
}
