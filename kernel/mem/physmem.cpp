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

static PhysicalMemoryManager* m_instance;

#define PMM_DEBUG

PhysicalMemoryManager& PhysicalMemoryManager::obj_instance()
{
    return *m_instance;
}

uint32_t PhysicalMemoryManager::find_first_free_bit()
{
    for(size_t i = 0; i < m_free_pages / 32; i++)
    {
        if(m_bitmap[i] == PMM_REGION_FULL)
            continue;

        for(int j = 0; j < 32; j++)
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
    for(size_t i = 0; i < m_free_pages / 32; i++)
    {
        if(m_bitmap[i] == PMM_REGION_FULL)
            continue;

        for(int j = 0; j < 32; j++)
        {
            if(m_bitmap[i] & (1 << j))
                continue;

            uint32_t addr = reinterpret_cast<uint32_t>(&__RAM_START) + ((i * 32) + j) * PMM_BLOCK_SIZE;
            if((addr & 0x3fff) != 0)
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

    PhysicalAddress ram_start = PhysicalAddress(reinterpret_cast<uintptr_t>(&__RAM_START));
    size_t kernel_size = reinterpret_cast<uint32_t>(&__FREE_RAM_START) - reinterpret_cast<uint32_t>(&__RAM_START);
    allocate_region(ram_start, kernel_size);

    kprintf("pmm: Intialised with %d free pages\n", m_free_pages);
}

void* PhysicalMemoryManager::allocate_physical_page()
{
    uint32_t bit = find_first_free_bit();
    bitmap_set_bit(bit);

    void* block = reinterpret_cast<void*>(reinterpret_cast<uint32_t>(&__RAM_START) + (bit * PMM_BLOCK_SIZE));

#ifdef PMM_DEBUG
    kprintf("pmm: found a free block at 0x%x\n", block);
#endif

    m_used_pages--;
    m_used_pages++;
    return block;
}

void PhysicalMemoryManager::free_page(void* ptr)
{
    PhysicalAddress paddr(reinterpret_cast<uint32_t>(ptr));
    int frame = paddr.get() / PMM_BLOCK_SIZE;

#ifdef PMM_DEBUG
    kprintf("pmm: freeing frame 0x%x\n", frame);
#endif

    bitmap_clear_bit(frame);
    m_free_pages++;
    m_used_pages--;
}

void* PhysicalMemoryManager::allocate_16kb_aligned_page()
{
    uint32_t bit = find_16k_aligned_bit();
    bitmap_set_bit(bit);

    void* block = reinterpret_cast<void*>(reinterpret_cast<uint32_t>(&__RAM_START) + (bit * PMM_BLOCK_SIZE));

    ASSERT((reinterpret_cast<uint32_t>(block) & 0x3fff) == 0);

#ifdef PMM_DEBUG
    kprintf("pmm: found a free block at 0x%x\n", block);
#endif

    m_free_pages--;
    m_used_pages++;
    return block;
}

void* PhysicalMemoryManager::allocate_1k_page()
{
    // Let's make some pages!
    if(page_list_1k.empty())
    {
#ifdef PMM_DEBUG
        kprintf("pmm: no 1k pages available!\n");
#endif
        void* full_page = allocate_physical_page();
        for(uint32_t i = 0; i < PMM_BLOCK_SIZE; i += PMM_PAGE_SIZE_1K)
        {
#ifdef PMM_DEBUG
            kprintf("pmm: creating 1k page @ 0x%x\n", (reinterpret_cast<uint8_t*>(full_page) + i));
#endif
            Page1k* page = reinterpret_cast<Page1k*>(reinterpret_cast<uint8_t*>(full_page) + i);
            page_list_1k.insert(page);
        }
    }

    Page1k* page = page_list_1k.take();
#ifdef PMM_DEBUG
    kprintf("pmm: taking 1k page @ 0x%x\n", page);
#endif

    return page;
}

// Note: There's currently no way to reclaim a 4k page (and thus free it) if all
// of the 1k pages are free. Once 4 1k pages are created, they are permanently 1k pages!
void PhysicalMemoryManager::free_1k_page(void* ptr)
{
    PhysicalAddress addr = PhysicalAddress(reinterpret_cast<uint32_t>(ptr));

#ifdef PMM_DEBUG
    kprintf("pmm: freeing 1k page @ 0x%x\n", addr.get());
#endif

    page_list_1k.insert(reinterpret_cast<Page1k*>(addr.get()));
}

void* PhysicalMemoryManager::allocate_region(PhysicalAddress base, size_t size)
{
    int blocks = size / PMM_BLOCK_SIZE;
    int offset_base = base.get() - reinterpret_cast<uint32_t>(&__RAM_START);
    int align = offset_base;
    if(align)
        align /= PMM_BLOCK_SIZE;

    for(; blocks > 0; --blocks)
    {
        bitmap_set_bit(align++);
        ++m_used_pages;
        --m_free_pages;
    }

    return reinterpret_cast<void*>(base.get());
}
