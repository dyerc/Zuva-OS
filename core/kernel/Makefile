CC = /usr/cross/bin/i586-elf-gcc
LD = /usr/cross/bin/i586-elf-ld
ASM = nasm

CFLAGS = -Wall -O2 -Wextra -nostdlib -nostdinc -nostartfiles -nodefaultlibs -ffreestanding -fno-strict-aliasing
INC = -I./include
DEFS= -DDEBUG -DARCH_X86

OBJS= \
arch/x86/asm/boot.o \
arch/x86/asm/i386.o \
arch/x86/asm/interrupt.o \
arch/x86/descriptors.o \
arch/x86/init.o \
arch/x86/io.o \
arch/x86/irq.o \
arch/x86/power.o \
arch/x86/rtc.o \
arch/x86/pit.o \
lib/utils.o \
lib/printf.o \
kernel.o \
console.o \
time.o

all: $(OBJS) link iso run

build: $(OBJS) link iso

.c.o:
	@echo Compiling: $<
	@$(CC) $(CFLAGS) $(INC) $(DEFS) -c $< -o $@
	
.s.o:
	@echo Assembling: $<
	@$(ASM) -f elf $< -o $@


link:
	@echo Linking...
	@$(LD) -T arch/linker.ld $(OBJS) $(x86emu) -o ../../build/iso/boot/kernel.bin

iso:
	cd ../../build; \
	genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o Zuva.iso iso

clean:
	rm $(OBJS)
	
run:
	cd C:/Code/qemu; \
	./qemu-win-zuva.bat
