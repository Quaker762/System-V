/**
 *  System-V Initial Boot Script
 *  
 *  At this point in boot, the processor has put us at
 *  0x10000 (which is 64KiB).
 */
.section .text.boot

.extern init
.extern __cxa_global_constructors

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

    bl __cxa_global_constructors // Call global constructors
    b init // Go to Kernel init

# If we end up here, the kernel's returned!! (not good!)
hang:
    b hang

