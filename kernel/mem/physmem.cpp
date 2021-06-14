/**
 * 
 */
#include <common/assert.h>
#include <kernel/kstdlib/kstdio.h>
#include <kernel/mem/address.h>
#include <kernel/mem/heap/heap.h>
#include <kernel/mem/physmem.h>
#include <stdint.h>

extern uint32_t __RAM_START;
extern uint32_t __RAM_END;
extern uint32_t __FREE_RAM_START;
extern uint32_t __L1_TABLE_POOL_START;
extern uint32_t __L1_TABLE_POOL_END;

static PhysicalMemoryManager* m_instance;

#define PMM_DEBUG

PhysicalMemoryManager& PhysicalMemoryManager::instance()
{
    return *m_instance;
}

uint32_t PhysicalMemoryManager::find_first_free_bit()
{
    for (size_t i = 0; i < m_free_pages / 32; i++)
    {
        if (m_bitmap[i] == PMM_REGION_FULL)
            continue;

        for (int j = 0; j < 32; j++)
        {
            if(m_bitmap[i] & (1 << j))
                continue;

            return (i * 32) + j;
        }
    }

    kprintf("WHOA! We've run out of physical memory! :(\n");
    ASSERT_NOT_REACHED();
}

// This is a bit of a hack...
uint32_t PhysicalMemoryManager::find_16k_aligned_bit()
{
    for (size_t i = 0; i < m_free_pages / 32; i++)
    {
        if (m_bitmap[i] == PMM_REGION_FULL)
            continue;

        for (int j = 0; j < 32; j++)
        {
            if(m_bitmap[i] & (1 << j))
                continue;

            uint32_t addr = reinterpret_cast<uint32_t>(&__RAM_START) + ((i * 32) + j) * PMM_BLOCK_SIZE;
            if ((addr & 0x3fff) != 0)
                continue;

            return (i * 32) + j;
        }
    }

    kprintf("WHOA! No more 16KiB aligned pages :( (no L1 table for you!)\n");
    ASSERT_NOT_REACHED();
}

PhysicalMemoryManager::PhysicalMemoryManager()
{
    init();
    m_instance = this;
}

void PhysicalMemoryManager::init()
{
    m_bitmap = reinterpret_cast<uint32_t*>(kmalloc_permanent(0x1000));
    m_free_bytes = reinterpret_cast<uint32_t>(&__RAM_END) - reinterpret_cast<uint32_t>(&__RAM_START);
    m_free_pages = m_free_bytes / PMM_BLOCK_SIZE;
    m_free_l1_tables = (reinterpret_cast<uint32_t>(&__L1_TABLE_POOL_END) - reinterpret_cast<uint32_t>(&__L1_TABLE_POOL_START)) / 0x4000;

    PhysicalAddress ram_start = PhysicalAddress(reinterpret_cast<uintptr_t>(&__RAM_START));
    size_t kernel_size = reinterpret_cast<uint32_t>(&__FREE_RAM_START) - reinterpret_cast<uint32_t>(&__RAM_START);
    allocate_region(ram_start, kernel_size);

    kprintf("pmm: Intialised with %d free pages\n", m_free_pages);
    kprintf("pmm: %d L1 Translation tables available\n", m_free_l1_tables);
}

PhysicalAddress PhysicalMemoryManager::allocate_l1_table(size_t index)
{
    if (m_free_l1_tables == 0)
        return PhysicalAddress(0);

    m_free_l1_tables--;
    return PhysicalAddress(reinterpret_cast<uint32_t>(&__L1_TABLE_POOL_START) + (index * 0x4000));
}

void* PhysicalMemoryManager::allocate_region(PhysicalAddress base, size_t size)
{
    int blocks = size / PMM_BLOCK_SIZE;
    int offset_base = base.get() - reinterpret_cast<uint32_t>(&__RAM_START);
    int align = offset_base;
    if(align)
        align /= PMM_BLOCK_SIZE;

    for (; blocks > 0; --blocks)
    {
        bitmap_set_bit(align++);
        ++m_used_pages;
        --m_free_pages;
    }

    return reinterpret_cast<void*>(base.get());
}
