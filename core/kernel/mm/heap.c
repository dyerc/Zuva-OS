
#include <mm.h>
#include <macros.h>
#include <printf.h>

extern mm_heap_t* kernel_heap;

mm_heap_t* mm_heap_create(uint32_t start, uint32_t limit)
{
	mm_heap_t* heap = (mm_heap_t*)mm_physical_alloc_dynamic(sizeof(mm_heap_t));
	
	heap->start = start;
	heap->end = limit;
	heap->size = limit - start;
	
	mm_page_alloc(current_context, heap->start, heap->end);
	memset((uint32_t*)start, 0x00, (limit - start));
	
	heap->first_chunk = (mm_chunk_t*)start;
	heap->first_chunk->magic = CHUNK_MAGIC;
	heap->first_chunk->status = CHUNK_FREE;
	heap->first_chunk->size = heap->size - sizeof(mm_chunk_t);
	heap->first_chunk->next = NULL;
	heap->first_chunk->previous = NULL;
	
	kprintf("Created Heap in region: [%X - %X] \n", start, limit);
	
	return heap;
}

void* mm_heap_malloc(mm_heap_t* heap, uint32_t size)
{
	if (!(mm_level & MM_HEAP))
		return 0;
		
	mm_chunk_t* header = heap->first_chunk;
	mm_chunk_t* header_dump = header;
	while(header)
	{
		if(header->magic == CHUNK_MAGIC)
		{
			if(header->status == CHUNK_FREE)
			{
				if(!header->next)
				{//this is the last header
					uint32_t addr = (uint32_t)header;
 					addr += sizeof(mm_chunk_t);
					header->status = CHUNK_ALLOCATED;
					header->size = size;

					uint32_t space_end = addr + size + sizeof(mm_chunk_t);
					if(space_end >= heap->end)
					{//heap expansion needed
						uint32_t old_end = heap->end;
						uint32_t new_end = space_end;
						new_end = (new_end & 0xFFFFF000) + PAGE_SIZE;

						if(!mm_page_alloc(current_context, old_end, new_end))
							return 0;

						heap->end = new_end;
					}

					//setup the new header
					header->next = (mm_chunk_t*)(addr + size);
					header->next->magic = CHUNK_MAGIC;
					header->next->status = CHUNK_FREE;
					header->next->size = 0;
					header->next->next = 0;

					return addr;
				}//endif(!header->next)
	
				if(header->size >= size)
				{
					uint32_t addr = (uint32_t)header;
					addr += sizeof(mm_chunk_t);
					header->status = CHUNK_ALLOCATED;
					memset((uint32_t*)addr, 0, size);	
					return addr;
				}				
			}//endif(header->free)

		if(!header->next)
			header_dump = header;		

		header = header->next;//advance to the next header
		}
		else
		{
			kprintf("Memory: Heap Corruption!\n");
			return 0;
		}
	}

	return 0;
}

void mm_heap_free(uint32_t ptr)
{
	if(!(mm_level & MM_HEAP))
		return;

	ASSERT(ptr > mm_free_start && ptr < mm_free_end);

	//mark the header as free
	mm_chunk_t* chunk = (mm_chunk_t*)((uint32_t)ptr - sizeof(mm_chunk_t));
	
	if(chunk->magic == CHUNK_MAGIC)
	{
		chunk->status = CHUNK_FREE;
	}
}

