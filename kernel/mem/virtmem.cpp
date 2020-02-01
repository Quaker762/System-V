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

void switch_L1_translation_table(L1TranslationTable* table)
{
    CP15::set_TTBR0(reinterpret_cast<uint32_t>(table));
}

L1TranslationTable* allocate_L1_translation_table()
{
    return reinterpret_cast<L1TranslationTable*>(MemoryManager::allocate_16kb_aligned_page());
}

} // namespace VirtMemoryManager
