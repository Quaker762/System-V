/**
 * 
 */
#pragma once

#include <kernel/arch/arm/cp15.h>
#include <kernel/arch/arm/cpu.h>
#include <kernel/mem/heap/heap.h>
#include <kernel/mem/physmem.h>
#include <stddef.h>
#include <stdint.h>

class VirtMemoryManager
{
    OBJ_PERMANENT
public:
    VirtMemoryManager() {}

    inline void switch_L1_translation_table(L1TranslationTable* table)
    {
        CPU::set_TTBR0(table);
    }

    inline L1TranslationTable* allocate_L1_translation_table()
    {
        return reinterpret_cast<L1TranslationTable*>(PhysicalMemoryManager::obj_instance().allocate_16kb_aligned_page());
    }

    void initialise_L1_translation_table(L1TranslationTable&);
    void initialise_L2_translation_table(L2TranslationTable&);

private:
    inline uint16_t L1_translation_table_index(VirtualAddress address)
    {
        return address.get() >> 20;
    }

    inline uint8_t L2_translation_table_index(VirtualAddress address)
    {
        return (address.get() >> 12) & 0xFF;
    }

    inline uint16_t page_index(VirtualAddress address)
    {
        return address.get() & 0xFFF;
    }
};
