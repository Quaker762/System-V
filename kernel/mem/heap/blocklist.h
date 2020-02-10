/**
 * 
 */
#pragma once

#include <stddef.h>

struct MemoryBlock
{
    size_t size;
    bool free;
    MemoryBlock* next;
};

struct MemoryBlockList
{
    MemoryBlock* head;
    MemoryBlock* tail;
    uint8_t pad[8];
};

void heap_list_init(void*);

void* heap_list_alloc(size_t);
void heap_list_free(void*);