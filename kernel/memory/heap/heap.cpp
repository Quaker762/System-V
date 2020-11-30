/*
 * Copyright (c) 2020 Jesse Buhagiar <jooster669@gmail.com>
 * Copyright (c) 2020 Matthew Atkin <>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <kernel/arch/arm/cpu.h>
#include <kernel/assertions.h>
#include <kernel/kstdlib/kstdio.h>
#include <kernel/memory/heap/blocklist.h>
#include <kernel/memory/heap/heap.h>
#include <stdint.h>

//#define KMALLOC_DEBUG

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
    kmalloc_permanent_ptr = reinterpret_cast<void*>(reinterpret_cast<uint32_t>(&__KMALLOC_BASE) + kmalloc_size);
    heap_list_init(kmalloc_ptr);
}

void* kmalloc_permanent(size_t size)
{
    if(reinterpret_cast<uint8_t*>(kmalloc_permanent_ptr) + size > reinterpret_cast<uint8_t*>(kmalloc_permanent_ptr) + kmalloc_permanent_size)
    {
        //kprintf("kmalloc: Out of permanent memory space!!! What the fuck!?\n");
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
