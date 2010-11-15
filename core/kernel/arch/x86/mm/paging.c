#include <arch/x86.h>
#include <mm.h>
#include <macros.h>
#include <printf.h>

uint32_t paging_frames_count;
uint32_t* paging_frames;

void paging_frame_alloc(uint32_t address)
{
	ASSERT((address % PAGE_SIZE) == 0);
	
	uint32_t frame_index = address / PAGE_SIZE;
	paging_frames[frame_index / 32] |= 0x1 << (frame_index % 32);
}

void paging_frame_free(uint32_t address)
{
	ASSERT((address % PAGE_SIZE) == 0);
	
	uint32_t frame_index = address / PAGE_SIZE;
	paging_frames[frame_index / 32] &= ~(0x0 << (frame_index % 32));
}

void paging_frame_reserve(uint32_t address)
{
	paging_frame_alloc(address);
}

uint32_t paging_frame_status(uint32_t address)
{
	ASSERT((address % PAGE_SIZE) == 0);
	
	uint32_t frame_index = address / PAGE_SIZE;
	return (paging_frames[frame_index / 32] & (0x1 << (frame_index % 32))) ? 1 : 0; /* 1 = Allocated, 0 = Free */
}

void paging_map_range(mm_context_t *context, uint32_t virtual_start, uint32_t virtual_end, uint32_t physical_address)
{
	if (mm_level == 0)
		return;
		
	uint32_t page_index_start = virtual_start / PAGE_SIZE;
	uint32_t page_index_end = (virtual_end / PAGE_SIZE) + 1;
	uint32_t page_index = page_index_start;
	uint32_t address = physical_address;
	uint32_t table_index = 0;
	
	for (page_index = page_index_start; page_index < page_index_end; page_index++)
	{
		table_index = page_index / 1024;
		
		if (!context->page_table[table_index])
		{
			mm_page_table_create(context, table_index);
		}
		
		context->page_table[table_index][page_index - (table_index * 1024)] = address | 3; 
		
		address += PAGE_SIZE;
	}
}

void mm_switch_context(mm_context_t* new_context)
{
	current_context = new_context;
	uint32_t cr0;
	__asm__ __volatile__("mov %0, %%cr3":: "b"(current_context->page_dir));
	__asm__ __volatile__("mov %%cr0, %0": "=b"(cr0));
	cr0 |= 0x80000000;
	__asm__ __volatile__("mov %0, %%cr0":: "b"(cr0));
}

void paging_disable()
{
	uint32_t cr0;
	__asm__ __volatile__("mov %%cr0, %0": "=b"(cr0));
	cr0 &= ~0x80000000;
	__asm__ __volatile__("mov %0, %%cr0":: "b"(cr0));
}
