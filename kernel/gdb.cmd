file kernel.elf
target remote | qemu-system-arm -S -gdb stdio -M virt -m 512 -cpu cortex-a15 -kernel kernel.elf
