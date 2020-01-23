.section .text.boot

.extern init
.extern __cxa_global_constructors

.global _start
_start:
    ldr sp, =__KERNEL_STACK_TOP
    bl __cxa_global_constructors
    b init

# If we end up here, the kernel's returned!! (not good!)
hang:
    b hang
