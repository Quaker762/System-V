/**
 * 
 */
#include <common/assert.h>
#include <kernel/kstdlib/kstdio.h>
#include <kernel/mem/address.h>
#include <kernel/mem/physmem.h>
#include <mjlib/linkedlist.h>
#include <stdint.h>

static uint32_t* m_bitmap = 0;
static size_t m_free_bytes = 0;
static size_t m_free_blocks = 0;
static size_t m_used_blocks = 0;

extern uint32_t __PMM_BITMAP;
extern uint32_t __USABLE_RAM_START;
extern uint32_t __RAM_BASE_ADDRESS;
extern uint32_t __RAM_TOP_ADDRESS;

static MJ::LinkedList<MemoryManager::Page1k> page_list_1k;

#define PMM_DEBUG

static inline void bitmap_set_bit(uint32_t bit)
{
    m_bitmap[bit / 32] |= (1 << (bit % 32));
}

static inline void bitmap_clear_bit(uint32_t bit)
{
    m_bitmap[bit / 32] &= ~(1 << (bit % 32));
}

static inline bool bitmap_test_bit(uint32_t bit)
{
    return static_cast<bool>(m_bitmap[bit / 32] & (1 << (bit % 32)));
}

static uint32_t find_first_free_bit()
{
    for(size_t i = 0; i < m_free_blocks / 32; i++)
    {
        if(m_bitmap[i] == MemoryManager::PMM_REGION_FULL)
            continue;

        for(int j = 0; j < 32; j++)
        {
            if(m_bitmap[i] & (1 << j))
                continue;

            return (i * 32) + j;
        }
    }

    ASSERT_NOT_REACHED();
}

// This is a bit of a hack...
static uint32_t find_16k_aligned_bit()
{
    for(size_t i = 0; i < m_free_blocks / 32; i++)
    {
        if(m_bitmap[i] == MemoryManager::PMM_REGION_FULL)
            continue;

        for(int j = 0; j < 32; j++)
        {
            if(m_bitmap[i] & (1 << j))
                continue;

            uint32_t addr = reinterpret_cast<uint32_t>(&__USABLE_RAM_START) + ((i * 32) + j) * MemoryManager::PMM_BLOCK_SIZE;
            if((addr & 0x3fff) != 0)
                continue;

            return (i * 32) + j;
        }
    }

    ASSERT_NOT_REACHED();
}

namespace MemoryManager
{

void init()
{
    m_bitmap = &__PMM_BITMAP;
    m_free_bytes = reinterpret_cast<uint32_t>(&__RAM_TOP_ADDRESS) - reinterpret_cast<uint32_t>(&__PMM_BITMAP);
    m_free_blocks = m_free_bytes / PMM_BLOCK_SIZE;

    kprintf("pmm: Intialised with %d free blocks\n", m_free_blocks);
}

void* allocate_physical_page()
{
    uint32_t bit = find_first_free_bit();
    bitmap_set_bit(bit);

    void* block = reinterpret_cast<void*>(reinterpret_cast<uint32_t>(&__USABLE_RAM_START) + (bit * PMM_BLOCK_SIZE));

#ifdef PMM_DEBUG
    kprintf("pmm: found a free block at 0x%x\n", block);
#endif

    m_used_blocks--;
    m_used_blocks++;
    return block;
}

void free_page(void* ptr)
{
    PhysicalAddress paddr(reinterpret_cast<uint32_t>(ptr));
    int frame = paddr.get() / PMM_BLOCK_SIZE;

#ifdef PMM_DEBUG
    kprintf("pmm: freeing frame 0x%x\n", frame);
#endif

    bitmap_clear_bit(frame);
    m_free_blocks++;
    m_used_blocks--;
}

void* allocate_16kb_aligned_page()
{
    uint32_t bit = find_16k_aligned_bit();
    bitmap_set_bit(bit);

    void* block = reinterpret_cast<void*>(reinterpret_cast<uint32_t>(&__USABLE_RAM_START) + (bit * PMM_BLOCK_SIZE));

    ASSERT((reinterpret_cast<uint32_t>(block) & 0x3fff) == 0);

#ifdef PMM_DEBUG
    kprintf("pmm: found a free block at 0x%x\n", block);
#endif

    m_free_blocks--;
    m_used_blocks++;
    return block;
}

void* allocate_1k_page()
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
void free_1k_page(void* ptr)
{
    PhysicalAddress addr = PhysicalAddress(reinterpret_cast<uint32_t>(ptr));

#ifdef PMM_DEBUG
    kprintf("pmm: freeing 1k page @ 0x%x\n", addr.get());
#endif

    page_list_1k.insert(reinterpret_cast<Page1k*>(addr.get()));
}

} // namespace MemoryManager
