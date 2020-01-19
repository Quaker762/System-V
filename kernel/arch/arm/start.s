.section .text.boot

.extern init

.global _start
_start:
    ldr sp, =__STACK_TOP
    b init

# If we end up here, the kernel's returned!! (not good!)
hang:
    b hang
