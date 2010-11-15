#include <arch/x86.h>
#include <mm.h>
#include <multiboot.h>
#include <utils.h>
#include <printf.h>
#include <irq.h>

extern uint32_t end;
extern void mm_pagefault_handle(registers_t regs);

uint32_t mm_free_start = (uint32_t)&end;
uint32_t mm_free_end;
uint32_t mm_total;

mm_heap_t* kernel_heap;
uint32_t mm_level = MM_NONE;

void* kmalloc(uint32_t size)
{
	return mm_heap_malloc(kernel_heap, size);
}

void kfree(void* ptr)
{
	mm_heap_free((uint32_t)ptr);
}

void mm_init(multiboot_t* multiboot)
{
	arch_register_irq_handler(14, (irq_t)&mm_pagefault_handle);
	
	mm_total = multiboot->mem_lower + multiboot->mem_upper;
	
	mm_free_end = mm_total * 1024;
	
	kernel_context = (mm_context_t*)mm_context_init();
	mm_context_switch(kernel_context);
	
	mm_level |= MM_VIRTUAL;
	
	paging_frames_count = mm_free_end / PAGE_SIZE;
	paging_frames = (uint32_t*)mm_physical_alloc_dynamic(sizeof(uint32_t) * (paging_frames_count / 32));
	
	mm_level |= MM_PHYSICAL;
	
	memset(paging_frames, (uint8_t)0xFFFFFFFF, (paging_frames_count / 32));
	
	// Page align the first usable memory location
	mm_free_start = (mm_free_start & 0xFFFFF000) + PAGE_SIZE;
	
	mmap_entry_t *mmap = (mmap_entry_t*)multiboot->mmap_address;
	uint32_t i = 0;
	
	while ((uint32_t)mmap < multiboot->mmap_address + multiboot->mmap_length)
	{
		if (mmap->type == 1)
		{
			for (i = mmap->base_addr_low; i < mmap->base_addr_low + mmap->length_low; i += PAGE_SIZE)
			{
				mm_physical_free(i);
			}
		}
		
		mmap = (mmap_entry_t*)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
	}
	
	/* Reserve all the memory that is unusable */
	for (i = 0; i < mm_free_start; i += PAGE_SIZE)
	{
		paging_frame_reserve(i);
	}

	kernel_heap = mm_heap_create(0x03304000, 0x08000000);
	
	mm_level |= MM_HEAP;
}
