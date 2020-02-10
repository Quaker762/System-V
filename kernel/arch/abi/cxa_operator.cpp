/**
 *  We don't use `new` just yet (because we have no Kernel heap allocator),
 *   
 */

#include <kernel/mem/heap/heap.h>

void* operator new(size_t alloc_size)
{
    return kmalloc(alloc_size);
}

void* operator new[](size_t alloc_size)
{
    return kmalloc(alloc_size);
}

void operator delete(void* ptr)
{
    kfree(ptr);
}

void operator delete(void* ptr, size_t)
{
    kfree(ptr);
}

void operator delete[](void* ptr, size_t)
{
    kfree(ptr);
}

void operator delete[](void* ptr)
{
    kfree(ptr);
}
