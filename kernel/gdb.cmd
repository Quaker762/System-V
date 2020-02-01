file kernel.elf
target remote | qemu-system-arm -S -gdb stdio -M vexpress-a15 -m 2G -cpu cortex-a15 -kernel kernel.elf
layout asm
