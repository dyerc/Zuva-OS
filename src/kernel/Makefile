CC = /usr/local/cross/bin/i586-elf-gcc
LD = /usr/local/cross/bin/i586-elf-ld
ASM = nasm

CFLAGS = -Wall -O2 -Wextra -nostdlib -nostdinc -nostartfiles -nodefaultlibs -ffreestanding -fno-strict-aliasing
INC = -I./include
DEFS= -DDEBUG -DARCH_X86

OBJS= \
arch/x86/entry.o \
arch/x86/gdt.o \
arch/x86/init.o \
arch/x86/io.o \
arch/x86/idt.o \
arch/x86/isr.o \
arch/x86/irq.o \
devices/pit.o \
lib/utils.o \
lib/printf.o \
kernel.o \
console.o

all: $(OBJS) link

build: $(OBJS) link

.c.o:
	@echo Compiling: $<
	@$(CC) $(CFLAGS) $(INC) $(DEFS) -c $< -o $@

.s.o:
	@echo Assembling: $<
	@$(ASM) -f elf $< -o $@


link:
	$(LD) -T arch/linker.ld $(OBJS) $(x86emu) -o ../../root/boot/zkernel

iso:
	cd ../../build; \
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o Zuva.iso iso

clean:
	rm $(OBJS)

run:
	cd C:/Code/qemu; \
	./qemu-zuva.bat
