/* 
 * kernel.c - The architecture CPU manager for x86
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

cpu_t cpu[MAX_CPUS];

void arch_cpu_init()
{
    uint32_t i;
    for (i = 0; i < MAX_CPUS; i++)
    {
	
    }
}