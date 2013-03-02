/*
 * paging.c - The paging functionality
 *
 *
 * This file is part of the Zuva Operating System.
 *
 * (C) 2013 Chris Dyer
 *
 */

#define KERNEL_HEAP_END 0x02000000

extern void *end;
uint32_t placement_pointer = (uint32_t)&end;
uint32_t heap_end = (uint32_t)NULL;

void kmalloc_startat(uint32_t address)
{
  placement_pointer = address;
}

uint32_t kmalloc_real(size_t size, int align, uint32_t *phys)
{
  if (heap_end)
  {
    void *address;

    if (align)
      address = valloc(size);
    else
      address = malloc(size);

    if (phys)
    {
      page_t *page = get_page((uint32_t)address, 0, kernel_directory);
      *phys = page->frame * 0x1000 + ((uint32_t)address & 0xFF);
    }

    return (uint32_t)address;
  }

  if (align && (placement_pointer & 0xFFFFF000))
  {
    placement_pointer &= 0xFFFFF000;
    placement_pointer += 0x1000;
  }

  if (phys)
    *phys = placement_pointer;

  uint32_t address = placement_pointer;
  placement_pointer += size;
  return (uint32_t)address;
}

uint32_t kmalloc(size_t size)
{
  return kmalloc_real(size, 0, NULL);
}

uint32_t kvmalloc(size_t size)
{
  return kmalloc_real(size, 1, NULL);
}

uint32_t kmalloc_p(size_t size, uint32_t *phys)
{
  return kmalloc_real(size, 0, phys);
}

uint32_t kvmalloc_p(size_t size, uint32_t *phys)
{
  return kmalloc_real(size, 1, phys);
}

/* Frames */

uint32_t *frames;
uint32_t nframes;

#define INDEX_FROM_BIT(b) (b / 0x20)
#define OFFSET_FROM_BIT(b) (b % 0x20)

void set_frame(uint32_t frame_addr)
{
  uint32_t frame  = frame_addr / 0x1000;
  uint32_t index  = INDEX_FROM_BIT(frame);
  uint32_t offset = OFFSET_FROM_BIT(frame);
  frames[index] |= (0x1 << offset);
}

void clear_frame(uint32_t frame_addr)
{
  uint32_t frame  = frame_addr / 0x1000;
  uint32_t index  = INDEX_FROM_BIT(frame);
  uint32_t offset = OFFSET_FROM_BIT(frame);
  frames[index] &= ~(0x1 << offset);
}

uint32_t test_frame(uint32_t frame_addr)
{
  uint32_t frame  = frame_addr / 0x1000;
  uint32_t index  = INDEX_FROM_BIT(frame);
  uint32_t offset = OFFSET_FROM_BIT(frame);
  return (frames[index] & (0x1 << offset));
}

uint32_t first_frame()
{
  uint32_t i, j;

  for (i = 0; i < INDEX_FROM_BIT(nframes); ++i)
  {
    if (frames[i] != 0xFFFFFFFF)
    {
      for (j = 0; j < 32; ++j)
      {
        uint32_t test_frame = 0x1 << j;
        if (!(frames[i] & test_frame))
          return i * 0x20 + j;
      }
    }
  }

  kprintf("System supposedly out of available memory");
  while(true) {}
  return -1;
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
  if (page->frame != 0)
  {
    page->present = 1;
    page->rw = (is_writeable == 1) ? 1 : 0;
    page->user = (is_kernel == 1) ? 0 : 1;
    return;
  }
  else
  {
    uint32_t index = first_frame();
    assert(index != (uint32_t)-1 && "Out of frames");

    (index * 0x1000);
    page->present = 1;
    page->rw = (is_writeable == 1) ? 1 : 0;
    page->user = (is_kernel == 1) ? 0 : 1;
    page->frame = index;
  }
}

void dma_frame(page_t *page, int is_kernel, int is_writeable, uint32_t address)
{
  page->present = 1;
  page->rw      = (is_writeable) ? 1 : 0;
  page->user    = (is_kernel)    ? 0 : 1;
  page->frame   = address / 0x1000;
}

void free_frame(page_t *page)
{
  uint32_t frame;
  if (!(frame = page->frame))
  {
    assert(0);
    return;
  }
  else
  {
    clear_frame(frame * 0x1000);
    page->frame = 0x0;
  }
}

uint32_t memory_use()
{

}

uint32_t memory_total()
{
  return nframes * 4;
}

void paging_install(uint32_t memsize)
{
  nframes = memsize / 4;
  frames = (uint32_t *)kmalloc(INDEX_FROM_BIT(nframes * 8));
  memset(frames, 0, INDEX_FROM_BIT(nframes));

  uint32_t phys;
  kernel_directory = (page_directory_t *)kvmalloc_p(sizeof(page_directory_t), &phys);
  memset(kernel_directory, 0, sizeof(page_directory_t));

  uint32_t i = 0;
  while (i < placement_pointer + 0x3000)
  {
    alloc_frame(get_page(i, 1, kernel_directory), 1, 0);
    i += 0x1000;
  }

  for (uint32_t j = 0xb8000; j < 0xc0000; j += 0x1000)
  {
    alloc_frame(get_page(j, 1, kernel_directory), 0, 1);
  }

  i
}

void switch_page_directory(page_directory_t *dir)
{
  current_directory = dir;
  asm volatile("mov %0, %%cr3":: "r"(dir->tablesPhysical));
  u32int cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t* get_page(uint32_t address, int make, page_directory_t *dir)
{

}

void page_fault(struct regs *r)
{

}
