/*
 * Copyright (c) 2020, Jesse Buhagiar <jooster669@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <kernel/assertions.h>
#include <kernel/kstdlib/kstdio.h>
#include <kernel/memory/heap/blocklist.h>

//#define HEAP_LIST_DEBUG

static MemoryBlockList* block_list = nullptr;
static void* list_base_address = nullptr;

// Find a block given a pointer
static MemoryBlock* heap_list_find_block(void* addr)
{
#ifdef HEAP_LIST_DEBUG
    kprintf("heap_list: ptr @ 0x%x\n", addr);
#endif
    MemoryBlock* block;

    for(block = block_list->head; block; block = block->next)
    {
#ifdef HEAP_LIST_DEBUG
        kprintf("heap_list: block: 0x%x block->next = 0x%x\n", block, block->next);
#endif
        if((reinterpret_cast<uint8_t*>(block) + sizeof(MemoryBlock)) == addr)
        {
#ifdef HEAP_LIST_DEBUG
            kprintf("heap_list: found block 0x%x for ptr 0x%x\n", block, addr);
#endif
            return block;
        }
    }

    ASSERT_NOT_REACHED();
}

static MemoryBlock* find_block(size_t size)
{
    size_t large = SIZE_MAX;
    MemoryBlock* block = nullptr;

    for(auto curr = block_list->head; curr; curr = curr->next)
    {
        if(curr->size < large && curr->size > size && curr->free)
        {
            large = curr->size;
            block = curr;
        }
    }

    return block;
}

void heap_list_init(void* heap_addr)
{
    block_list = reinterpret_cast<MemoryBlockList*>(heap_addr);
    block_list->head = nullptr;
    block_list->tail = nullptr;

    list_base_address = heap_addr;
}

void* heap_list_alloc(size_t alloc_size)
{
    MemoryBlock* block = nullptr;

    if(block_list->head == nullptr)
    {
#ifdef HEAP_LIST_DEBUG
        kprintf("heap_list: no blocks in list!\n");
#endif
        block = reinterpret_cast<MemoryBlock*>(reinterpret_cast<uint8_t*>(list_base_address) + sizeof(MemoryBlockList));
        block->size = alloc_size;
        block->free = false;
        block->next = nullptr;

        block_list->head = block;
        block_list->tail = block;
#ifdef HEAP_LIST_DEBUG
        kprintf("heap_list: allocated a block @ 0x%x\n", block);
#endif
        return reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(block) + sizeof(MemoryBlock));
    }

    block = find_block(alloc_size);

    if(block == nullptr)
    {
#ifdef HEAP_LIST_DEBUG
        kprintf("heap_list: unable to find suitable block for size %d; creating\n", alloc_size);
#endif
        ASSERT(block_list->tail != nullptr); // Definitely not good...
        block = reinterpret_cast<MemoryBlock*>(reinterpret_cast<uint8_t*>(block_list->tail) + block_list->tail->size);
        block->free = false;
        block->next = nullptr;
        block->size = alloc_size;

        block_list->tail->next = block;
        block_list->tail = block;
#ifdef HEAP_LIST_DEBUG
        kprintf("heap_list: created a new block @ 0x%x\n", block);
#endif
        return reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(block) + sizeof(MemoryBlock));
    }

    // Let's split it
    if(block->size > alloc_size)
    {
#ifdef HEAP_LIST_DEBUG
        kprintf("heap_list: block->size > allo_size, splitting block\n");
#endif
        MemoryBlock* split = nullptr;
        size_t diff = block->size - alloc_size;

        // This inserts the split block to the RIGHT of the current block
        split = reinterpret_cast<MemoryBlock*>(reinterpret_cast<uint8_t*>(block) + alloc_size);
        split->free = true;
        split->size = diff;
        split->next = block->next;

        block->size = alloc_size;
        block->next = split;
        block->free = false;
    }

#ifdef HEAP_LIST_DEBUG
    kprintf("heap_list: found block @ 0x%x(%d)\n", block, block->size);
#endif

    return reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(block) + sizeof(MemoryBlock));
}

void heap_list_free(void* ptr)
{
    MemoryBlock* block = heap_list_find_block(ptr);
    block->free = true;
}
