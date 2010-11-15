
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
	
	/*heap->first_chunk = (mm_chunk_t*)start;
	heap->first_chunk->magic = CHUNK_MAGIC;
	heap->first_chunk->status = CHUNK_FREE;
	heap->first_chunk->size = heap->size - sizeof(mm_chunk_t);
	heap->first_chunk->next = NULL;
	heap->first_chunk->previous = NULL;*/
	
	heap->first_block = (mm_block_t*)start;
	heap->first_block->magic = BLOCK_MAGIC;
	heap->first_block->size = BLOCK_DEFAULT_SIZE;
	heap->first_block->next = NULL;
	heap->first_block->previous = NULL;
	
	heap->first_block->first_chunk = (mm_chunk_t*)start + sizeof(mm_block_t);
	heap->first_block->first_chunk->magic = CHUNK_MAGIC;
	heap->first_block->first_chunk->status = CHUNK_FREE;
	heap->first_block->first_chunk->size = heap->first_block->size - sizeof(mm_block_t) - sizeof(mm_chunk_t);
	heap->first_block->first_chunk->block = heap->first_block;
	heap->first_block->first_chunk->unused = 0;
	heap->first_block->first_chunk->next = NULL;
	heap->first_block->first_chunk->previous = NULL;
	
	heap->first_block->biggest_free = heap->first_block->first_chunk->size;
	
	kprintf("Created Heap in region: [%X - %X] \n", start, limit);
	
	return heap;
}

inline mm_block_t* mm_heap_generate_block(mm_heap_t* heap, uint32_t size)
{
	uint32_t block_size = BLOCK_DEFAULT_SIZE;
	mm_block_t* last_block = heap->first_block;
	mm_block_t* new_block;
	
	while (last_block)
		last_block = last_block->next;
		
	if ((size + sizeof(mm_block_t) + sizeof(mm_chunk_t)) > block_size)
		block_size = (size + sizeof(mm_block_t) + sizeof(mm_chunk_t));
	
	if (heap->end > ((uint32_t)last_block + last_block->size + block_size))
		return NULL;
		
	new_block = last_block + last_block->size;
	new_block->size = block_size;
	new_block->magic = BLOCK_MAGIC;
	new_block->next = NULL;
	new_block->previous = last_block;
	last_block->next = new_block;
	
	new_block->first_chunk = (mm_chunk_t*)(new_block + sizeof(mm_block_t));
	new_block->first_chunk->magic = CHUNK_MAGIC;
	new_block->first_chunk->status = CHUNK_FREE;
	new_block->first_chunk->size = (block_size - sizeof(mm_block_t) - sizeof(mm_chunk_t));
	new_block->first_chunk->block = new_block;
	new_block->first_chunk->unused = 0;
	new_block->first_chunk->next = NULL;
	new_block->first_chunk->previous = NULL;
	
	new_block->biggest_free = new_block->first_chunk->size;
}

inline void mm_heap_recalc_block_free(mm_block_t* block)
{
	mm_chunk_t* current_chunk = block->first_chunk;
	uint32_t largest_free = 0;
		
	/* Re-find which is the largest free chunk in the block */
	while (current_chunk)
	{
		if (current_chunk->status == CHUNK_FREE && current_chunk->size > largest_free)
		{
			largest_free = current_chunk->size;
		}
			
		current_chunk = current_chunk->next;
	}
		
	block->biggest_free = largest_free;
}

mm_chunk_t* mm_heap_glue_chunk(mm_chunk_t* chunk)
{
	if (chunk->status == CHUNK_FREE)
	{
		if (chunk->next != NULL && chunk->next->status == CHUNK_FREE)
		{
			mm_chunk_t* next = chunk->next;
			chunk->size += next->size + sizeof(mm_chunk_t);
			
			chunk->next = next->next;
			next->next->previous = chunk;
			
			//memset(next, 0, (sizeof(mm_chunk_t) + next->size));
		}
		
		if (chunk->previous != NULL && chunk->previous->status == CHUNK_FREE)
		{
			mm_chunk_t* previous = chunk->previous;
			previous->size += chunk->size + sizeof(mm_chunk_t);
			
			previous->next = chunk->next;
			chunk->next->previous = previous;
			
			//memset(chunk, 0, (sizeof(mm_chunk_t) + chunk->size));
			chunk = previous;
		}
	}
	
	return chunk;
}

mm_chunk_t* mm_heap_split_chunk(mm_chunk_t* chunk, uint32_t size)
{
	mm_chunk_t* new_chunk = chunk + size;
	
	new_chunk->size = (chunk->next - (new_chunk + sizeof(mm_chunk_t)));
	new_chunk->magic = CHUNK_MAGIC;
	new_chunk->status = CHUNK_FREE;
	new_chunk->block = chunk->block;
	new_chunk->unused = 0;
	new_chunk->next = chunk->next;
	new_chunk->previous = chunk;
	
	chunk->next = new_chunk;
	new_chunk->next->previous = new_chunk;
	
	mm_heap_glue_chunk(new_chunk->next); // Just try it
	
	return new_chunk;
}

void* mm_heap_alloc_chunk(mm_chunk_t* chunk, uint32_t size)
{
	ASSERT(chunk->magic == CHUNK_MAGIC);
	
	chunk->status = CHUNK_ALLOCATED;
	
	/*if (chunk->size > (size + sizeof(mm_chunk_t)))
	{
		chunk->unused = 0;
		mm_heap_split_chunk(chunk, size);
	}
	else
	{
		chunk->unused = (chunk->size - (size + sizeof(mm_chunk_t)));
	}*/
	
	mm_heap_recalc_block_free(chunk->block);
	
	return (void*)(chunk);
}

void* mm_heap_alloc_block(mm_block_t* block, uint32_t size)
{
	mm_chunk_t* chunk = block->first_chunk;
	
	while (chunk)
	{
		if (chunk->size >= size && chunk->status == CHUNK_FREE)
		{
			kprintf("%d\n", chunk);
			return mm_heap_alloc_chunk(chunk, size);
		}
		
		chunk = chunk->next;
	}
	
	// Should never happen
	return (void*)0;
}

void* mm_heap_malloc(mm_heap_t* heap, uint32_t size)
{
	if (!(mm_level & MM_HEAP))
		return 0;
		
	mm_block_t* block = heap->first_block;
	
	while (block)
	{
		if (block->biggest_free >= size)
		{
			return mm_heap_alloc_block(block, size);
		}
		
		block = block->next;
	}
	
	/* No block has enough space... make a new one */
	block = mm_heap_generate_block(heap, size);
	
	if (block)
	{
		return mm_heap_alloc_block(block, size);
	}
	else
	{
		/* HEAP FULL! */
		kprintf("Heap Full!!!\n");
		return (void*)0;
	}
}

void mm_heap_free(uint32_t ptr)
{
	mm_chunk_t* chunk = (mm_chunk_t*)((uint32_t)ptr - sizeof(mm_chunk_t));
	kprintf("%d\n", chunk);
	
	ASSERT(chunk->magic == CHUNK_MAGIC);
	
	chunk->status = CHUNK_FREE;
	chunk = mm_heap_glue_chunk(chunk);
	chunk->status = CHUNK_FREE;

	mm_heap_recalc_block_free(chunk->block);
}

