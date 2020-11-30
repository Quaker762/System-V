/*
 * Copyright (c) 2020 Jesse Buhagiar <jooster669@gmail.com>
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
 *
 *  System-V Initial Boot code
 *  
 *  At this point in boot, the processor has put us at
 *  0x10000 (which is 64KiB).
 */
 
 .set PAGE_DIRECTORY_ENTRY_TYPE, 2
 .set PAGE_DIRECTORY_BUFFER_BIT, 4
 .set PAGE_DIRECTORY_ENTRY_CACHE_BIT, 8
 .set PAGE_DIRECTORY_ENTRY_COMPAT_BIT, 0x10
 .set PAGE_DIRECTORY_ENTRY_DOMAIN, 0
 .set PAGE_DIRECTORY_ENTRY_PERMISSIONS, 0x400 /* Kernel Mode only */


.section .text.boot

.extern init
.extern __cxa_global_constructors

uart0_addr:
    .long 0xd01f1000

.global _start
_start:
    # Let's set up all the stacks for different modes!
    msr cpsr, #0x93 // Set the CPU mode to SUPERVISOR, interrupts disabled
    ldr sp, =__KERNEL_STACK_TOP

    # Switch to ABORT mode
    msr cpsr, #0x97 // Set CPU mode to ABORT, interrupts DISABLED
    ldr sp, =__ABORT_STACK_TOP

    # Switch to UNDEFINED mode
    msr cpsr, #0x9B // Set CPU mode to ABORT, interrupts DISABLED
    ldr sp, =__ABORT_STACK_TOP

    # Switch to IRQ mode
    msr cpsr, #0x92 // Set CPU mode to IRQ, interrupts DISABLED
    ldr sp, =__IRQ_STACK_TOP

    # Switch back to Supervisor (Kernel) mode with interrupts DISABLED
    msr cpsr, #0x93

/**
 * Okay, now we need to set up some paging stuff. The kernel's virtual
 * address space starts at 3GiB + 1MiB (0xc0100000). This address *obviously*
 * doesn't actually exist in physical memory (it's probably some peripheral
 * or a device), so we need to create a few pages and map them so we don't
 * crash. 
 *
 * In our linker script, we have .text.* (minus .text.boot of course!) loaded
 * at VA 0xc0100000, while it's actual load address is at the physical address
 * in memory. We use 1MiB ARM paging "sections" to map the kernel only (we know
 * how big it is from the linker script) to the higher address space, then branch
 * to the initialization routines (__cxa_global_constructors and init)
 */
create_boot_page_directory:
    ldr r0, =boot_page_directory
    mov r1, #0/* Value to write */
    mov r2, #(3073 * 4) /* First entry is at offset 3073 */
    ldr r3, =__KERNEL_IMAGE_START_ADDRESS
    ldr r4, =__KERNEL_IMAGE_END_ADDRESS

    /**
     * Our entries look like the following:
     *
     * bit0:1:      10 (this entry is a 'section' entry)
     * bit2:        0 (no write buffer)
     * bits5:8      0 (Domain 0 [???])
     * bits11:10    10 (reads/writes in User Mode cause a page fault)
     * bits19:12    0 (always 0)
     * bits31:20    Physical Address mapped to this section frame
     *
     * r1 is the register used for the entry
    */
1:
    eor r1, r1
    add r1, #0x100000
    orr r1, $PAGE_DIRECTORY_ENTRY_PERMISSIONS
    orr r1, $PAGE_DIRECTORY_ENTRY_TYPE
    
    str r1, [r0, r2] /* Put the entry in the table */
    add r2, #4
    add r3, #0x100000
    cmp r3, r4
    blt 1b

    /**
     * We *also* need to identity map the first meg of memory, otherwise the code below the following
     * section will generate a page fault, as .text.boot is linked to address 0x10000.
     */
    eor r1, r1
    orr r1, $PAGE_DIRECTORY_ENTRY_PERMISSIONS
    orr r1, $PAGE_DIRECTORY_ENTRY_TYPE
    str r1, [r0]

map_peripherals:
    /**
     * Oh yeah, we also need to map the peripheral block so we can extract
     * *some* kind of debug information from the kernel via the UART in the
     * early stages of boot. We could *technically* do this in the Kernel itself,
     * but there's a LOT of bootstrapping required that creates a chicken/egg debug
     * scenario...
     *
     * On this board the peripheral block is located at physical address 0x10000000.
     * It's 2MiB in length, so we'll map two 1MiB sections to 0xd0000000 (kernel
     * virtual address space + 0x10000000)
     *
     * These entries reside at entries 0xd00 and 0xd01 in the Page Directory
     */
    eor r1, r1
    mov r2, #(0xd00 * 4)
    adds r1, #0x10000000
    orr r1, $PAGE_DIRECTORY_ENTRY_PERMISSIONS
    orr r1, $PAGE_DIRECTORY_ENTRY_TYPE
    str r1, [r0, r2]

    adds r1, #0x100000
    adds r2, #4
    str r1, [r0, r2]

enable_mmu:
    /* Let's write the Level 1 table to the correct register and actually enable the MMU! */
    /* r0 contains boot page directory */
    mcr p15, #0, r0, c2, c0, #0 /* Write the table's physical address */
    mcr p15, #0, r0, c8, c7, #0 /* Flush TLB */

    mov r0, #1
    mcr p15, #0, r0, c3, c0, #0

    /* Enable the MMU */
    mov r0, #1
    mrc p15, #0, r0, c1, c0, #0
    orr r0, #0x00000001
    mcr p15, #0, r0, c1, c0, #0
    nop
    nop
    nop
    nop
    mrc p15, 0, r2, c1, c0, 0
    mov r2, r2

    bl __cxa_global_constructors // Call global constructors
    b init // Go to Kernel init

# If we end up here, the kernel's returned!! (not good!)
hang:
    b hang
