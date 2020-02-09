/**
 * 
 */
#include <kernel/mem/address.h>
#include <kernel/mem/virtmem.h>

void VirtMemoryManager::initialise_L1_translation_table(L1TranslationTable& table)
{
    for(uint32_t i = 0; i < ENTIRES_PER_L1_TRANSLATION_TABLE; ++i)
    {
        table.entries[i].always_1 = 0b1;
        table.entries[i].always_0 = 0b0;
        table.entries[i].pxn = 0b0;
        table.entries[i].ns = 0b0;
        table.entries[i].sbz = 0b0;
        table.entries[i].domain = 0b0000;
        table.entries[i].unused = 0b0;
    }
}

void VirtMemoryManager::initialise_L2_translation_table(L2TranslationTable& table)
{
    for(uint32_t i = 0; i < ENTIRES_PER_L2_TRANSLATION_TABLE; ++i)
    {
        table.entries[i].xn = 0b0;
        table.entries[i].always_1 = 0b1;
        table.entries[i].b = 0b0;
        table.entries[i].c = 0b0;
        table.entries[i].ap_lo = 0b11;
        table.entries[i].tex = 0b000;
        table.entries[i].ap_hi = 0b0;
        table.entries[i].s = 0b0;
        table.entries[i].nG = 0b0;
    }
}
