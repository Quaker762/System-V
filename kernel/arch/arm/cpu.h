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
#pragma once

#include <stdint.h>

#define CAUSE_ILLEGAL_INSTRUCTION __asm__ volatile("bad:\n.word 0xffffffff\n")
#define CAUSE_DATA_ABORT __asm__ volatile("mov r0,#0x00\nldr r0, [r0, #-4]")
#define CAUSE_PREFETCH_ABORT __asm__ volatile("bkpt")

[[noreturn]] static inline void halt()
{
    __asm__ volatile("cpsid iaf"); // Disable all interrupts (IRQ, FIQ and **)
    __asm__ volatile("wfi");       // Wait for an interrupt (which can never happen!)
    for(;;)
        ;
}

// Disable FIQ and IRQs
static inline void cli()
{
    __asm__ volatile("cpsid iaf"); // Disable all interrupts (IRQ, FIQ and **)
}

static inline void sti()
{
    __asm__ volatile("cpsie iaf");
}

struct RegisterDump
{
    uint32_t spsr;
    uint32_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, fp, r12, sp, lr;
} __attribute__((packed));
