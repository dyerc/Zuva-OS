/*
 * irq.c - The x86 IRQ implementation
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
#include <irq.h>

#define PIT_A 0x40
#define PIT_B 0x41
#define PIT_C 0x42
#define PIT_CONTROL 0x43

#define TIMER_IRQ 0

#define PIT_MASK 0xFF
#define PIT_SCALE 1193180
#define PIT_SET 0x36

uint32_t tick = 0;

void timer_handler(struct registers *regs)
{
  irq_ack(TIMER_IRQ);

  tick++;
  kprintf("Tick: %d\n", tick);
}

void timer_phase(uint32_t hz)
{
  int divisor = PIT_SCALE / hz;

  outb(PIT_CONTROL, PIT_SET);
  outb(PIT_A, (uint8_t)(divisor & PIT_MASK));
  outb(PIT_A, (uint8_t)((divisor >> 8) & PIT_MASK));
}

void timer_install()
{
  irq_install_handler(TIMER_IRQ, timer_handler);
  timer_phase(50);

  kprintf("[K] Interval timer initialized\n");
}
