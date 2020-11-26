qemu-system-arm -m 128M \
    -M versatilepb \
    -cpu cortex-a8 \
    -kernel kernel.elf \
    -serial mon:stdio 
