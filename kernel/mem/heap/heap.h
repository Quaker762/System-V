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

[[gnu::nonnull]] void* kmalloc_permanent(size_t) __attribute__((malloc));
[[gnu::nonnull]] void* kmalloc(size_t) __attribute__((malloc));
[[gnu::nonnull]] void* kmalloc_aligned(size_t) __attribute__((malloc));
void kfree(void*);