/**
 *  ARM level 1 page table structure
 */
#pragma once

#include <mem/physmem.h>
#include <mem/vm/physpage.h>

class Level1TranslationTable final
{
public:
    Level1TranslationTable();
    ~Level1TranslationTable();

    uint32_t ttrb0() const { return m_table_address.get(); }
    uint32_t entry(int index) const { return reinterpret_cast<uint32_t*>(m_table_address.get())[index]; }

private:
    PhysicalAddress m_table_address { 0 }; // Address of

};