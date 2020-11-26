file kernel.elf
target remote | qemu-system-arm -S -gdb stdio -m 128M -M versatilepb -cpu cortex-a8 -kernel kernel.elf
layout asm
