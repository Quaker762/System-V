/**
 * 
 */
#include <kernel/arch/arm/cpu.h>
#include <kernel/assertions.h>
#include <kernel/kstdlib/kstdio.h>
#include <kernel/mem/heap/blocklist.h>
#include <kernel/mem/heap/heap.h>
#include <stdint.h>

#define KMALLOC_DEBUG

extern uintptr_t __KMALLOC_BASE;
static void* kmalloc_ptr = nullptr;
static void* kmalloc_permanent_ptr = nullptr;

static constexpr size_t kmalloc_size = 2 * (1024 * 1024);
static constexpr size_t kmalloc_permanent_size = kmalloc_size;

/////////////////////////////////////////////////////////////////////////////////

void kmalloc_init()
{
#ifdef KMALLOC_DEBUG
    kprintf("kmalloc initialized! __KMALLOC_BASE @ 0x%x\n", &__KMALLOC_BASE);
#endif
    kmalloc_ptr = reinterpret_cast<void*>(&__KMALLOC_BASE);
    kmalloc_permanent_ptr = reinterpret_cast<void*>(&__KMALLOC_BASE + kmalloc_size);
    heap_list_init(kmalloc_ptr);
}

void* kmalloc_permanent(size_t size)
{
    if(reinterpret_cast<uint8_t*>(kmalloc_permanent_ptr) + size > reinterpret_cast<uint8_t*>(kmalloc_permanent_ptr) + kmalloc_permanent_size)
    {
        kprintf("kmalloc: Out of permanent memory space!!! What the fuck!?\n");
        cli();
        for(;;)
            ;
    }

    void* ptr = kmalloc_permanent_ptr;
#ifdef KMALLOC_DEBUG
    kprintf("kmalloc_permanent: handing out pointer @ 0x%x, size %d\n", reinterpret_cast<uint32_t>(ptr), size);
#endif
    kmalloc_permanent_ptr = reinterpret_cast<uint8_t*>(ptr) + size;

    return ptr;
}

void* kmalloc(size_t alloc_size)
{
    return heap_list_alloc(alloc_size);
}

void* kmalloc_aligned(size_t)
{
    ASSERT_NOT_REACHED();
}

void kfree(void* ptr)
{
    heap_list_free(ptr);
}
