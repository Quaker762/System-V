/**
 * 
 */
#include <kernel/assertions.h>
#include <kernel/kstdlib/kstdio.h>
#include <kernel/mem/heap.h>
#include <stdint.h>

//#define KMALLOC_DEBUG

extern void* __KMALLOC_BASE;
static void* kmalloc_ptr = nullptr;
static void* kmalloc_permanent_ptr = nullptr;

static constexpr size_t kmalloc_size = 2 * (1024 * 1024);
static constexpr size_t kmalloc_permanent_size = kmalloc_size;

void kmalloc_init()
{
#ifdef KMALLOC_DEBUG
    kprintf("kmalloc initialized! __KMALLOC_BASE @ 0x%x\n", &__KMALLOC_BASE);
#endif
    kmalloc_ptr = reinterpret_cast<void*>(&__KMALLOC_BASE);
    kmalloc_permanent_ptr = reinterpret_cast<void*>(&__KMALLOC_BASE + kmalloc_size);
}

void* kmalloc_permanent(size_t size)
{
    void* ptr = kmalloc_permanent_ptr;
#ifdef KMALLOC_DEBUG
    kprintf("kmalloc_permanent: handing out pointer @ 0x%x, size %d\n", reinterpret_cast<uint32_t>(ptr), size);
#endif
    kmalloc_permanent_ptr = ptr + size;

    return ptr;
}

void* kmalloc(size_t)
{
    ASSERT_NOT_REACHED();
}

void* kmalloc_aligned(size_t)
{
    ASSERT_NOT_REACHED();
}
