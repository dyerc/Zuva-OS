/* 
 * pages.c - The physical architecture independent page manager.
 * 
 * 
 * This file is part of the Zuva Operating System. 
 * 
 * (C) Copyright Chris Dyer & Zuvium 2009 - 2010. Do Not Re-distribute. 
 * For internal viewing only.
 * 
 */

#include <mm.h>
#include <macros.h>
#include <printf.h>

uint32_t mm_physical_alloc(uint32_t size)
{
	if (mm_level & MM_PHYSICAL)
	{
		ASSERT(size < mm_free_end);
		
		/* Find a free frame */
		uint32_t i, j;
		uint32_t free_size = 0, free_start = 0;
		
		for (i = 0; i < mm_free_end; i += PAGE_SIZE)
		{
			if (paging_frame_status(i) == 0)
			{
				if (free_size == 0)
				{
					free_start = i;
					free_size += PAGE_SIZE;
				}
				else
				{
					free_size += PAGE_SIZE;
				}
			
				if (free_size >= size)
				{
					for (j = free_start; j < (free_start + free_size); j += PAGE_SIZE)
					{
						paging_frame_alloc(j);
					}
			
					return free_start;
				}
			}
			else
			{
				free_size = 0;
				free_start = 0;
			}
		}
	}
	else
	{
		uint32_t address = mm_free_start;
		
		if(address & 0x00000FFF)
		{
			address += PAGE_SIZE;
			address &= 0xFFFFF000;
		}
		
		mm_free_start = address + size;
		
		return address;
	}
}

/* Same as mm_page_alloc but for the current address space */
uint32_t mm_physical_alloc_dynamic(uint32_t size)
{
	uint32_t physical_address = mm_physical_alloc(size);
	
	paging_map_range(current_context, physical_address, physical_address + size, physical_address);
		
	return physical_address;
}

void* mm_page_alloc(mm_context_t *context, uint32_t start, uint32_t end)
{
	uint32_t physical_address = mm_physical_alloc(end - start);
	
	if (!physical_address)
		return 0;
	
	paging_map_range(context, start, end, physical_address);
		
	return (void*)physical_address;
}

void mm_physical_free(uint32_t ptr)
{
	if (ptr >= mm_free_end)
	{
		//kprintf("Ptr = %X,    MM_FREE_END = %X\n\n", ptr, mm_free_end);
		ASSERT(ptr >= mm_free_end);
	}
	
	paging_frame_free(ptr);
}

void mm_page_free(mm_context_t* context, uint32_t start, uint32_t end)
{
	uint32_t index_start = start / PAGE_SIZE;
	uint32_t index_end = end / PAGE_SIZE;
	uint32_t table_index, page_index;
	uint32_t physical_address;
	
	for (page_index = index_start; page_index < index_end; page_index++)
	{
		table_index = page_index / 1024;
		
		if (context->page_table[table_index])
		{
			physical_address = context->page_table[table_index][page_index - (table_index * 1024)] & 0xFFFFF000;

			context->page_table[table_index][page_index - (table_index * 1024)] = 0;
			
			mm_physical_free(physical_address);
		}
	}
}

void mm_page_table_create(mm_context_t* context, uint32_t table_index)
{
	if (!(mm_level > 0))
		return;
		
	//kprintf("Creating Page Table for: %X --> Tbl Index = %d", (uint32_t)context, table_index);
	
	uint32_t address = mm_physical_alloc(1024);
	
	context->page_dir[table_index] = address;
	context->page_dir[table_index] |= 3;
	
	context->page_table[63][table_index] = address | 3;
	context->page_table[table_index] = (uint32_t*)(PAGE_TABLE_START + (table_index * PAGE_SIZE));
	
	uint32_t i;
	for (i = 0; i < 1024; i++)
	{
		context->page_table[table_index][i] = 0;
	}
}
