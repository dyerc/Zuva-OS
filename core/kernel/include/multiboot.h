#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <types.h>

typedef struct
{
	uint32_t flags;

    uint32_t memory_lower;
    uint32_t memory_upper;

    uint32_t boot_device;

    const char* kernel_parameters;

    uint32_t module_count;
    void* first_module;

    union {
        struct {
            uint32_t tabsize;
            uint32_t strsize;
            uint32_t addr;
            uint32_t reserved;
        } aout_info;

        struct {
            uint32_t num;
            uint32_t size;
            uint32_t addr;
            uint32_t shndx;
        } elf_info;
    };

    uint32_t memory_map_length;
    uint32_t memory_map_address;
	
} multiboot_header_t;

#endif
