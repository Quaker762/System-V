/**
 * 
 */
#include <kernel/mem/virtmem.h>

namespace VirtMemoryManager
{

static uint16_t L1_translation_table_index(VirtualAddress address)
{
    return address.get() >> 20;
}

static uint8_t L2_translation_table_index(VirtualAddress address)
{
    return (address.get() >> 12) & 0xFF;
}

static uint16_t page_index(VirtualAddress address)
{
    return address.get() & 0xFFF;
}

static void set_TTBR0(uint32_t L1TT_base)
{
    __asm__ volatile("mcr p15, 0, %[value], c2, c0, 0"
                     :
                     : [value] "r"(L1TT_base));
}

static uint32_t get_TTBR0()
{
    uint32_t reg;

    __asm__ volatile("mrc p15, 0, %[result], c2, c0, 0"
                     : [result] "=r"(reg));

    return reg;
}

uint32_t get_TTBCR()
{
    uint32_t reg;

    __asm__ volatile("mrc p15, 0, %[result], c2, c0, 2"
                     : [result] "=r"(reg));

    return reg;
}

void switch_L1_translation_table(L1_translation_table* table)
{
    set_TTBR0(reinterpret_cast<uint32_t>(table));
}

L1_translation_table* allocate_L1_translation_table()
{
    return static_cast<L1_translation_table*>(MemoryManager::allocate_16kb_aligned_page());
}

} // namespace VirtMemoryManager
