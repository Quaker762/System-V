ENTRY(_start)

SECTIONS {
    . = 0x00000000;
    __PHYSICAL_MEMORY_START = .;

    . = 0x10000;
    __KERNEL_START_ADDRESS = .;

    .text BLOCK(4K) : ALIGN(4K)
    {
        __START_OF_KERNEL_TEXT = .;
        *(.text.boot)
        *(.text)
        *(.text.*)
        __END_OF_KERNEL_TEXT = .;
    }

    . = ALIGN(4);
    .init_array :
    {
        __init_array_start = .;
        KEEP (*(.init_array*))
        __init_array_end = .;
    }

    __KERNEL_END = .;

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(.bss.COMMON)
    }

}
