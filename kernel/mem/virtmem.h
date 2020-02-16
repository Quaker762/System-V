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
    VirtMemoryManager();

    inline void switch_L1_translation_table(L1TranslationTable* table)
    {
        CPU::set_TTBR0(table);
        m_current_table = table;
    }

    inline L1TranslationTable* allocate_L1_translation_table()
    {
        return reinterpret_cast<L1TranslationTable*>(PhysicalMemoryManager::obj_instance().allocate_16kb_aligned_page());
    }

    void map_address(PhysicalAddress phys_addr, VirtualAddress virt_addr);

    inline L1TranslationTable* get_current_table() { return m_current_table; }

private:
    inline uint16_t l1_translation_table_index(VirtualAddress address)
    {
        return address.get() >> 20;
    }

    inline uint8_t l2_translation_table_index(VirtualAddress address)
    {
        return (address.get() >> 12) & 0xFF;
    }

    inline uint16_t page_index(VirtualAddress address)
    {
        return address.get() & 0xFFF;
    }

    inline L1TableEntry* get_l1_entry(L1TranslationTable* table, VirtualAddress addr)
    {
        return &table->entries[l1_translation_table_index(addr)];
    }

    inline L2TableEntry* get_l2_entry(L2TranslationTable* table, VirtualAddress addr)
    {
        return &table->entries[l2_translation_table_index(addr)];
    }

    void identity_map_kernel();
    void map_devices();
    void allocate_page(L2TableEntry& entry);
    void free_page(L2TableEntry& entry);

private:
    L1TranslationTable* m_current_table;
};
