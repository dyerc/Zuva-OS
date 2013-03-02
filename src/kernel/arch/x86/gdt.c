/*
 * gdt.c - The x86 GDT implementation
 *
 *
 * This file is part of the Zuva Operating System.
 *
 * (C) 2013 Chris Dyer
 *
 */

#include <arch/x86.h>
#include <utils.h>
#include <types.h>

typedef struct
{
   uint16_t limit;
   uint32_t base;

} __attribute__((packed)) descriptor_ptr_t;

typedef struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;

} __attribute__((packed)) gdt_entry_t;

descriptor_ptr_t gdt_ptr;
gdt_entry_t gdt[5];

extern void arch_gdt_flush();

void arch_gdt_set_gate(uint32_t num, unsigned long base, unsigned long limit, uint8_t access, uint8_t gran)
{
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void arch_gdt_install()
{
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 3) - 1;
    gdt_ptr.base = (uint32_t)&gdt;

    arch_gdt_set_gate(0, 0, 0, 0, 0);
    arch_gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    arch_gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    arch_gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    arch_gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    arch_gdt_flush();

    kprintf("[x86] GDT initialized\n");
}
