#ifndef IRQ_H
#define IRQ_H

typedef void (*irq_handler_t) (struct registers *);

extern void isr_install_handler(int isrs, irq_handler_t);
extern void isr_uninstall_handler(int isrs);

extern void irq_install_handler(int irq, irq_handler_t);

#endif
