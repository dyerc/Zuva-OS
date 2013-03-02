#ifndef MM_H
#define MM_H

#include <arch/x86.h>
#include <types.h>
#include <lib/map.h>
#include <multiboot.h>

typedef struct {
	uint32_t present:1;
	uint32_t rw:1;
	uint32_t user:1;
	uint32_t accessed:1;
	uint32_t dirty:1;
	uint32_t unused:7;
	uint32_t frame:20;
} page_t;

typedef struct {
	page_t pages[1024];
} page_table_t;

typedef struct {
	page_table_t *tables[1024];
	uint32_t physical_tables[1024];
	uint32_t physical_address;

	int32_t ref_count;
} page_directory_t;


#define CHUNK_MAGIC			0x12
#define BLOCK_MAGIC			0xCD

#define BLOCK_DEFAULT_SIZE	0x1000

#define PAGE_SIZE			0x1000
#define PAGE_TABLE_START 	0x0FC00000
#define PAGE_TABLE_END		0x0FFFFFFF

#define MM_NONE	    	0
#define MM_PHYSICAL		1
#define MM_VIRTUAL		2
#define MM_HEAP 		4

/* Represents a virtual address space */
typedef struct
{
	uint32_t* page_dir;
	uint32_t* page_table[1024];

} mm_context_t;

/* Abstracts a section of memory in use within a block */

typedef enum
{
	CHUNK_FREE,
	CHUNK_ALLOCATED

} mm_chunk_status;

struct mm_block;

typedef struct mm_chunk
{
	uint8_t magic;
	uint16_t unused;
	uint32_t size; // Doesnt include size of mm_chunk
	mm_chunk_status status;

	//struct mm_block* block;
	struct mm_chunk* next;
	struct mm_chunk* previous;

} mm_chunk_t;

// TODO: Reimplement
/* Abstracts a section of memory */
/*typedef struct mm_block
{
	//mm_chunk_t *first_chunk;

	uint32_t size; // Includes size of block itself or first chunk size
	uint32_t biggest_free;
	uint8_t magic;

	struct mm_block* next;
	struct mm_block* previous;

} mm_block_t;*/

typedef struct
{
	uint32_t start;
	uint32_t end;
	uint32_t size;

	mm_chunk_t* first_chunk;

} mm_heap_t;


extern uint32_t mm_free_end;
extern uint32_t mm_free_start;

extern uint32_t paging_frames_count;
extern uint32_t* paging_frames;

extern mm_context_t* kernel_context;
extern mm_context_t* current_context;

extern uint32_t mm_level;

/****************** Heap *******************/

mm_heap_t* mm_heap_create(uint32_t start, uint32_t limit);
void* mm_heap_malloc(mm_heap_t* heap, uint32_t size);
void mm_heap_free(uint32_t ptr);

/******* Manager (Global Functions) *******/

void* kmalloc(uint32_t size);
void kfree(void* ptr);

void mm_init(multiboot_t* multiboot);

void mm_pagefault_handle(registers_t regs);

#endif
