# Defines a table for the Interrupt Vector Table.
# The address of this table is loaded into the GIC.VBAR
# coprocessor register

/**
.global vector_table
vector_table:
    .weak reset_handler
    .weak undefined_instruction
    .weak system_call
    .weak prefetch_abort
    .weak data_abort
    .weak UNUSED
    .weak irq_handler
    .weak fiq_handler
**/
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

.extern set_VBAR
.global relocate_vector_table
.section .text
relocate_vector_table:
    ldr r0, =vector_table
    stmfd sp!, {r0-r12, lr}
    bl set_VBAR
    ldmfd sp!, {r0-r12, pc}^
    bx lr

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
    stmfd sp!, {r0-r12, lr}
    mrs r0, spsr
    stmfd sp!, {r0}
    mov r0, sp

    # Jump to the actual exception handler
    bl illegal_instruction_handler
    add sp, sp, #4 // Ignore the value of SPSR we pushed onto the stack
    ldmfd sp!, {r0-r12, pc}^ // Return from exception

.extern data_abort_handler
prefetch_abort_trampoline:
    # Let's save all the registers before we enter the cpp function handler
    SUB  lr, lr, #4 // Adjust LR as per table above
    stmfd sp!, {r0-r12, lr}
    mrs r0, spsr
    stmfd sp!, {r0}
    mov r0, sp

    # Jump to the actual exception handler
    bl prefetch_abort_handler
    add sp, sp, #4 // Ignore the value of SPSR we pushed onto the stack
    ldmfd sp!, {r0-r12, pc}^ // Return from exception

.extern data_abort_handler
data_abort_trampoline:
    # Let's save all the registers before we enter the cpp function handler
    SUB  lr, lr, #8 // Adjust LR as per table above
    stmfd sp!, {r0-r12, lr}
    mrs r0, spsr
    stmfd sp!, {r0}
    mov r0, sp

    # Jump to the actual exception handler
    bl data_abort_handler
    add sp, sp, #4 // Ignore the value of SPSR we pushed onto the stack
    ldmfd sp!, {r0-r12, pc}^ // Return from exception

.extern handle_irq
irq_trampoline:
    msr cpsr, #0x93 // Switch back to Kernel Mode (SVC) with interrupts disabled

    # Let's save all the registers before we enter the cpp function handler
    stmfd sp!, {r0-r12, lr}
    mrs r0, cpsr
    stmfd sp!, {r0}

    push {sp} // Push the stack pointer to the regs list

    # Jump to the C irq handler
    mov r0, sp
    bl handle_irq

    # Re-enable interrupts and return to whatever was happening before
    mrs r0, cpsr
    bic r0, #0x80
    msr cpsr, r0

    ldr sp, [sp]
    add sp, sp, #4
    ldmfd sp!, {r0-r12, pc}^

.extern syscall_handler
svc_trap:
    // Okay, so at this point, r0-r3 contains syscall arguments.
    // These are put here by the CPU automagically, which is
    // nice!
    // However, we have to load up the syscall number by extracting
    // it from the instruction found in the link register minus 4
    ldr r4, [lr, #-4]
    bic r4, r4, #0xff000000

    stmfd sp!, {r0-r12, lr} // Save some state
    mrs r0, spsr
    stmfd sp!, {r0}

    push {sp}

    mov r0, sp
    bl syscall_handler

    add sp, sp, #8
    ldmfd sp!, {r0-r12, pc}^
