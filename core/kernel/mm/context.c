#include <mm.h>

mm_context_t* kernel_context = NULL;
mm_context_t* current_context; 


mm_context_t* mm_context_init()
{
	mm_context_t* context;
	
	if (mm_level > 0) // <-- We need virtual mem
		context = mm_physical_alloc_dynamic(sizeof(mm_context_t));
	else
		context = mm_physical_alloc(sizeof(mm_context_t));
		
	memset(context, 0, sizeof(mm_context_t));
	
	/********************* INIT ROUTINE ***********************/
	
	if (mm_level > 0)
		context->page_dir = (uint32_t*)mm_physical_alloc_dynamic(1024);
	else
		context->page_dir = (uint32_t*)mm_physical_alloc(1024);
		
	uint32_t i;
	for (i = 0; i < 1024; i++)
	{
		context->page_dir[i] = 0;
	}
	
	uint32_t address;
	if (mm_level > 0)
		address = mm_physical_alloc_dynamic(1024);
	else
		address = mm_physical_alloc(1024);
		
	uint32_t page_index = PAGE_TABLE_START / PAGE_SIZE;
	uint32_t table_index = 63; /* Stole this from the fluidium memory manager, sorry */
	context->page_table[table_index] = (uint32_t*)address;
	
	for (i = 0; i < 1024; i++)
	{
		context->page_table[table_index][i] = 0;
	}
	
	context->page_dir[table_index] = address;
	context->page_dir[table_index] |= 3;
	
	context->page_table[table_index][page_index - (table_index * 1024)] = address | 3;
	
	page_index = 0x00 / PAGE_SIZE;
	
	if (mm_level > 0)
		address = mm_physical_alloc_dynamic(1024);
	else
		address = mm_physical_alloc(1024);
		
	context->page_table[0] = (uint32_t*)address;
	
	for (i = 0; i < 1024; i++)
	{
		context->page_table[0][i] = 0;
	}
	
	context->page_dir[0] = address;
	context->page_dir[0] |= 3;
	
	context->page_table[63][0] = address | 3;
	
	uint32_t page_index_start = 0 / PAGE_SIZE;
	uint32_t page_index_end = (mm_free_start / PAGE_SIZE) + 1;
	address = 0;
	
	for (page_index = page_index_start; page_index < page_index_end; page_index++)
	{
		table_index = page_index / 1024;
		
		context->page_table[table_index][page_index - (table_index * 1024)] = address | 3;
		address += PAGE_SIZE;
	}
	
	context->page_table[table_index] = (uint32_t*)(PAGE_TABLE_START + (63 * PAGE_SIZE));
	
	context->page_table[0] = (uint32_t*)(PAGE_TABLE_START);
	
	/**********************************************************/
	
	if (kernel_context)
	{
		table_index = 0;
		page_index = 0;
		
		for (table_index = 0; table_index < 256; table_index++)
		{
			if (table_index == 63)
				continue;
			
			if (kernel_context->page_table[table_index])
			{
				address = mm_physical_alloc_dynamic(1024);
				
				context->page_dir[table_index] = address;
				context->page_dir[table_index] |= 3;
				
				context->page_table[63][table_index] = address | 3;
				context->page_table[table_index] = (uint32_t*)address;
				
				for (page_index = 0; page_index < 1024; page_index++)
				{
					if (kernel_context->page_table[table_index][page_index])
					{
						context->page_table[table_index][page_index] = kernel_context->page_table[table_index][page_index];
					}
				}
				
				context->page_table[table_index] = (uint32_t*)(PAGE_TABLE_START + (table_index * PAGE_SIZE));
			}
		}
	}
	
	return context;
}

void mm_context_destory(mm_context_t* context)
{
	uint32_t i;
	for (i = 0; i < 1024; i++)
	{
		if (context->page_dir[i])
		{
			mm_physical_free(context->page_dir[i]);
		}
	}
	
	mm_physical_free((uint32_t)context);
}

void mm_context_switch(mm_context_t* context)
{
	current_context = context;
	uint32_t cr0;
	__asm__ __volatile__("mov %0, %%cr3":: "b"(current_context->page_dir));
	__asm__ __volatile__("mov %%cr0, %0": "=b"(cr0));
	cr0 |= 0x80000000;
	__asm__ __volatile__("mov %0, %%cr0":: "b"(cr0));
}
