/**
 * 
 */
#pragma once

#include <kernel/mem/heap/heap.h>
#include <mjlib/linkedlist.h>
#include <stddef.h>
#include <stdint.h>

struct Page1k
{
    Page1k* next;
};

class PhysicalMemoryManager
{
    OBJ_PERMANENT
private:
    static constexpr uint32_t PMM_BLOCK_SIZE = 0x1000;
    static constexpr uint32_t PMM_PAGE_SIZE_1K = 0x400;
    static constexpr uint32_t PMM_REGION_FULL = 0xffffffff;

public:
    PhysicalMemoryManager();

    void* allocate_physical_page();
    void free_page(void*);
    void* allocate_1k_page();
    void free_1k_page(void*);
    void* allocate_16kb_aligned_page();
    void* allocate_region(PhysicalAddress, size_t);

    static PhysicalMemoryManager& obj_instance();

private:
    void init();

    uint32_t find_first_free_bit();
    uint32_t find_16k_aligned_bit();

    inline void bitmap_set_bit(uint32_t bit)
    {
        m_bitmap[bit / 32] |= (1 << (bit % 32));
    }

    inline void bitmap_clear_bit(uint32_t bit)
    {
        m_bitmap[bit / 32] &= ~(1 << (bit % 32));
    }

    inline bool bitmap_test_bit(uint32_t bit)
    {
        return static_cast<bool>(m_bitmap[bit / 32] & (1 << (bit % 32)));
    }

private:
    MJ::LinkedList<Page1k> page_list_1k;
    size_t m_free_pages { 0 };
    size_t m_used_pages { 0 };
    size_t m_free_bytes { 0 };
    uint32_t* m_bitmap { nullptr };
};