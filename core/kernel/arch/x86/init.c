/* 
 * init.c - The architecture initialisation point for the
 * x86 (i386) processor.
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

#include <kernel.h>
#include <multiboot.h>
#include <printf.h>
#include <utils.h>

multiboot_header_t *multiboot;
extern uint32_t end;

void arch_init(multiboot_header_t *header)
{
	memcpy(multiboot, header, sizeof(multiboot_header_t));
	
    // Init the descriptors
    
    arch_gdt_install();
    
    // Init interrupts
    
    arch_idt_install();
    
    // Init paging
    
    // Go to architecture independent init point of kernel
    kernel_init();
}

void arch_shutdown()
{
}
