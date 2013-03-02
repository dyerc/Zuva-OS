/*
 * idt.c - The x86 IDT implementation
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
  uint16_t base_low;
  uint16_t sel;
  uint8_t zero;
  uint8_t flags;
  uint16_t base_high;

} __attribute__((packed)) idt_entry_t;

typedef struct
{
  uint16_t limit;
  uint32_t base;

} __attribute__((packed)) idt_ptr_t;

idt_entry_t idt[256];
idt_ptr_t idtp;

extern void arch_idt_load();

void arch_idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
  idt[num].base_low = (base & 0xFFFF);
  idt[num].base_high = (base >> 16) & 0xFFFF;
  idt[num].sel = sel;
  idt[num].zero = 0;
  idt[num].flags = flags/* | 0x60*/;
}

void arch_idt_install()
{
  idtp.limit = (sizeof(idt_entry_t) * 256) - 1;
  idtp.base = (uint32_t)&idt;
  memset(&idt, 0, sizeof(idt_entry_t) * 256);

  arch_idt_load();

  kprintf("[x86] IDT initialized\n");
}
