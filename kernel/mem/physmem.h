/**
 * 
 */
#pragma once

#include <kernel/mem/heap/heap.h>
#include <kernel/mem/vm/physpage.h>
#include <qtl/linkedlist.h>
#include <stddef.h>
#include <stdint.h>

class PhysicalMemoryPage;

class PhysicalMemoryManager
{
    OBJ_PERMANENT
private:
    static constexpr uint32_t PMM_BLOCK_SIZE = 0x1000;
    static constexpr uint32_t PMM_PAGE_SIZE_1K = 0x400;
    static constexpr uint32_t PMM_REGION_FULL = 0xffffffff;

public:
    static PhysicalMemoryManager& instance();

public:
    PhysicalMemoryManager();

    void* allocate_region(PhysicalAddress, size_t);
    PhysicalAddress allocate_l1_table(size_t index);

private:
    void init();

    uint32_t find_first_free_bit();
    uint32_t find_16k_aligned_bit();

    void bitmap_set_bit(uint32_t bit)
    {
        m_bitmap[bit / 32] |= (1 << (bit % 32));
    }

    void bitmap_clear_bit(uint32_t bit)
    {
        m_bitmap[bit / 32] &= ~(1 << (bit % 32));
    }

    bool bitmap_test_bit(uint32_t bit)
    {
        return static_cast<bool>(m_bitmap[bit / 32] & (1 << (bit % 32)));
    }

private:
    qtl::LinkedList<PhysicalMemoryPage> page_list_1k;
    qtl::LinkedList<PhysicalMemoryPage> page_list_4k;
    size_t m_free_pages { 0 };
    size_t m_used_pages { 0 };
    size_t m_free_bytes { 0 };
    size_t m_free_l1_tables { 0 };
    uint32_t* m_bitmap { nullptr };
};
