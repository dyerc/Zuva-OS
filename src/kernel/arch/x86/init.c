/*
 * init.c - The architecture initialisation point for the
 * x86 (i386) processor.
 *
 *
 * This file is part of the Zuva Operating System.
 *
 * (C) 2013 Chris Dyer
 *
 */

#include <arch/x86.h>

#include <kernel.h>
#include <multiboot.h>
#include <printf.h>
#include <mm.h>
#include <utils.h>

multiboot_t *multiboot;

void arch_init(multiboot_t *mboot)
{
	memcpy(multiboot, mboot, sizeof(multiboot_t));

  // Init the descriptors
  console_init();

  arch_gdt_install();
  arch_idt_install();
  arch_isr_install();
  arch_irq_install();

  // Go to architecture independent init point of kernel
  kernel_init();
}

void arch_shutdown()
{
}
