qemu-system-arm -m 512 \
    -M virt \
    -cpu cortex-a15 \
    -kernel kernel.elf \
    -device VGA,vgamem_mb=32
