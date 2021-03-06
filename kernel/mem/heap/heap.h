/**
 * 
 */
#pragma once

#include <stddef.h>

#define OBJ_PERMANENT                                             \
public:                                                           \
    void*                                                         \
    operator new(size_t size) { return kmalloc_permanent(size); } \
                                                                  \
private:

void kmalloc_init();

[[gnu::returns_nonnull]] void* kmalloc_permanent(size_t) __attribute__((malloc, alloc_size(1)));
[[gnu::returns_nonnull]] void* kmalloc(size_t) __attribute__((malloc, alloc_size(1)));
[[gnu::returns_nonnull]] void* kmalloc_aligned(size_t) __attribute__((malloc, alloc_size(1)));
void kfree(void*);