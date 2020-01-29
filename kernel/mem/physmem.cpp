/**
 * 
 */
#include <common/assert.h>
#include <kernel/kstdlib/kstdio.h>
#include <kernel/mem/physmem.h>
#include <stdint.h>

static uint32_t* m_bitmap = 0;
static size_t m_free_bytes = 0;
static size_t m_free_blocks = 0;
static size_t m_used_blocks = 0;

extern uint32_t __PMM_BITMAP;
extern uint32_t __USABLE_RAM_START;
extern uint32_t __RAM_BASE_ADDRESS;
extern uint32_t __RAM_TOP_ADDRESS;

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

#ifdef DEBUG
    kprintf("pmm: found a free block at 0x%x\n", block);
#endif

    return block;
}

} // namespace MemoryManager
