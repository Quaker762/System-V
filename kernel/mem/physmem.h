/**
 * 
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace MemoryManager
{

struct Page1k
{
    Page1k* next;
};

static constexpr uint32_t PMM_BLOCK_SIZE = 0x1000;
static constexpr uint32_t PMM_PAGE_SIZE_1K = 0x400;
static constexpr uint32_t PMM_REGION_FULL = 0xffffffff;

void init();
void* allocate_physical_page();
void free_page(void*);
void* allocate_1k_page();
void free_1k_page();
void* allocate_16kb_aligned_page();
void* allocate_region(size_t);

} // namespace MemoryManager