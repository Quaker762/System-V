/**
 * 
 */
#pragma once

#include <kernel/kstdlib/kstring.h>
#include <kernel/mem/address.h>
#include <kernel/mem/physmem.h>
#include <stddef.h>
#include <stdint.h>

static constexpr uint32_t ENTIRES_PER_L2_TRANSLATION_TABLE = 256;
static constexpr uint32_t ENTIRES_PER_L1_TRANSLATION_TABLE = 4096;
static constexpr uint32_t PAGE_SIZE = 4096;

typedef uint32_t L2TT_entry;
typedef uint32_t L1TT_entry;

struct L2_translation_table
{
    L2TT_entry entries[ENTIRES_PER_L2_TRANSLATION_TABLE];
};

struct L1_translation_table
{
    L1TT_entry entries[ENTIRES_PER_L1_TRANSLATION_TABLE];
};

namespace VirtMemoryManager
{
uint32_t get_TTBCR();
L1_translation_table* allocate_L1_translation_table();
void switch_L1_translation_table(L1_translation_table* table);

} // namespace VirtMemoryManager
