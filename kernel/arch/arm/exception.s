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
 */
.section .text.vector_table
.global vector_table
.align 8
vector_table:
    ldr pc, =_start
    b illegal_instruction_trampoline
    b svc_trap
    b prefetch_abort_trampoline
    b data_abort_trampoline
    b .
    b irq_trampoline
    b .

/**
LR needs to be adjusted for when we want to return after the exception has been handled.

The following table is defined in the ARM Architecture Reference Manual:

-----------------------------------------------------------------------
EXCEPTION        ADJUSTMENT        INSTRUCTION RETURNED TO
-----------------------------------------------------------------------
SVC              0                 Next Instruction
Undef(illegal)   0                 Next Instruction
Prefetch Abort   -4                Aborting Instruction
Data Abort       -8                Aborting Instruction if precise
FIQ              -4                Next Instruction
IRQ              -4                Next Instruction
------------------------------------------------------------------------
*/

.extern illegal_instruction_handler
illegal_instruction_trampoline:
    # We need to reload the stack pointer. More about
    # why here: https://electronics.stackexchange.com/questions/291548/undefined-exception-in-arm-processor

    # Let's save all the registers before we enter the cpp function handler
    stmfd sp, {r0-r12, sp, lr}
    sub sp, sp, #60
    mrs r0, spsr
    stmfd sp!, {r0}
    mov r0, sp

    # Jump to the actual exception handler
    //bl illegal_instruction_handler
    add sp, sp, #4 // Ignore the value of SPSR we pushed onto the stack
    ldmfd sp, {r0-r12, sp, pc}^ // Return from exception

.extern data_abort_handler
prefetch_abort_trampoline:
    # Let's save all the registers before we enter the cpp function handler
    stmfd sp, {r0-r12, sp, lr}
    sub sp, sp, #60
    mrs r0, spsr
    stmfd sp!, {r0}
    mov r0, sp
    b .

    # Jump to the actual exception handler
    //bl prefetch_abort_handler
    add sp, sp, #4 // Ignore the value of SPSR we pushed onto the stack
    ldmfd sp, {r0-r12, sp, pc}^ // Return from exception

.extern data_abort_handler
data_abort_trampoline:
    # Let's save all the registers before we enter the cpp function handler
    SUB  lr, lr, #8 // Adjust LR as per table above
    stmfd sp, {r0-r12, sp, lr}
    sub sp, sp, #60
    mrs r0, spsr
    stmfd sp!, {r0}

    # Jump to the actual exception handler
    //bl data_abort_handler
    add sp, sp, #4 // Ignore the value of SPSR we pushed onto the stack
    ldmfd sp, {r0-r12, sp, pc}^ // Return from exception

.extern handle_irq
irq_trampoline:
    msr cpsr, #0xD3 // Switch back to Kernel Mode (SVC) with interrupts disabled
    mrs lr, lr_irq // We need to save this
    sub lr, lr, #4

    # Let's save all the registers before we enter the cpp function handler
    stmfd sp, {r0-r12, sp, lr}
    sub sp, sp, #60
    mrs r0, cpsr
    stmfd sp!, {r0}

    # Jump to the C irq handler
    mov r0, sp
    //bl handle_irq

    add sp, sp, #4
    ldmfd sp, {r0-r12, sp, pc}^

.extern syscall_handler
svc_trap:
    // Okay, so at this point, r0-r3 contains syscall arguments.
    // These are put here by the CPU automagically, which is
    // nice!
    // However, we have to load up the syscall number by extracting
    // it from the instruction found in the link register minus 4
    ldr r4, [lr, #-4]
    bic r4, r4, #0xff000000

    stmfd sp, {r0-r12, sp, lr}
    sub sp, sp, #60
    mrs r0, spsr
    stmfd sp!, {r0}

    mov r0, sp
    //bl syscall_handler

    add sp, sp, #4
    ldmfd sp, {r0-r12, sp, pc}^
