/**
 * 
 */
#pragma once

#include <kernel/arch/arm/cp15.h>
#include <kernel/kstdlib/kstring.h>
#include <kernel/mem/address.h>
#include <kernel/mem/physmem.h>
#include <stddef.h>
#include <stdint.h>

static constexpr uint32_t ENTIRES_PER_L2_TRANSLATION_TABLE = 256;
static constexpr uint32_t ENTIRES_PER_L1_TRANSLATION_TABLE = 4096;
static constexpr uint32_t PAGE_SIZE = 4096;

union L1TableEntry {
    struct
    {
        uint32_t always_1 : 1;
        uint32_t always_0 : 1;
        uint32_t pxn : 1;
        uint32_t ns : 1;
        uint32_t sbz : 1;
        uint32_t domain : 4;
        uint32_t unused : 1;
        uint32_t table_base_address : 22;
    };
    uint32_t entry;
};

union L2TableEntry {
    struct
    {
        uint32_t xn : 1;
        uint32_t always_1 : 1;
        uint32_t b : 1;
        uint32_t c : 1;
        uint32_t ap_lo : 2;
        uint32_t tex : 3;
        uint32_t ap_hi : 1;
        uint32_t s : 1;
        uint32_t nG : 1;
        uint32_t page_base_address : 20;
    };
    uint32_t entry;
};

struct L2TranslationTable
{
    L2TableEntry entries[ENTIRES_PER_L2_TRANSLATION_TABLE];
};

struct L1TranslationTable
{
    L1TableEntry entries[ENTIRES_PER_L1_TRANSLATION_TABLE];
};

namespace VirtMemoryManager
{

L1TranslationTable* allocate_L1_translation_table();
void switch_L1_translation_table(L1TranslationTable* table);

} // namespace VirtMemoryManager
