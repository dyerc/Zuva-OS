#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#define MB_HEADER_MAGIC     0x1BADB002
#define MB_BOOTLOADER_MAGIC 0x2BADB002

#define MB_FLAG_ALIGN_MODULES 0x0001
#define MB_FLAG_MEMORY_INFO   0x0002
#define MB_FLAG_ELF_SYM_INFO  0x0020

#ifndef __ASSEMBLER__

#include <types.h>

typedef struct
{
  uint32_t flags;
  uint32_t mem_lower;
  uint32_t mem_upper;
  uint32_t boot_device;
  uint32_t cmdline;
  uint32_t mods_count;
  uint32_t mods_addr;
  uint32_t num;
  uint32_t size;
  uint32_t addr;
  uint32_t shndx;
  uint32_t mmap_length;
  uint32_t mmap_address;
  uint32_t drives_length;
  uint32_t drives_addr;
  uint32_t config_table;
  uint32_t boot_loader_name;
  uint32_t apm_table;
  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint32_t vbe_mode;
  uint32_t vbe_interface_seg;
  uint32_t vbe_interface_off;
  uint32_t vbe_interface_len;
} __attribute__((packed)) multiboot_t;

typedef struct
{
  uint32_t size;
  uint32_t base_addr_low;
  uint32_t base_addr_high;
  uint32_t length_low;
  uint32_t length_high;
  uint32_t type;
} __attribute__((packed)) mmap_entry_t;

#endif

#endif
