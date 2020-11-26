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
    add r0, r0, #5
    add r0, r0
    
hang:
    b hang
