#include <mm.h>
#include <irq.h>
#include <printf.h>

void mm_pagefault_handle(registers_t regs)
{
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
   
	kprintf("PAGE FAULT: [%X]\n\n", faulting_address);
	
	asm volatile("hlt");
}
