/*
 * kernel.c - The main architecture independant entry point of the
 * kernel. Called after the architecture has initialised.
 *
 *
 * This file is part of the Zuva Operating System.
 *
 * (C) 2013 Chris Dyer
 *
 */

#include <types.h>
#include <kernel.h>
#include <printf.h>
#include <mm.h>
#include <console.h>
#include <multiboot.h>

extern multiboot_t *multiboot;

void kernel_init()
{
  timer_install();
}
