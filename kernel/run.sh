qemu-system-arm -m 2G \
    -M vexpress-a15 \
    -cpu cortex-a15 \
    -kernel kernel.elf \
    -serial mon:stdio
