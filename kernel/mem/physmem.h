/**
 * 
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace MemoryManager
{

static constexpr uint32_t PMM_BLOCK_SIZE = 0x1000;
static constexpr uint32_t PMM_REGION_FULL = 0xffffffff;

void init();
void* allocate_physical_page();
void* allocate_region(size_t);

}