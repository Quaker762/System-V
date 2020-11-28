ENTRY(_start)

SECTIONS {
    . = 0x00000000;
    __PHYSICAL_MEMORY_START = .;

    . = 0x00010000;
    __KERNEL_IMAGE_START_ADDRESS = .;

    .text BLOCK(4K) : ALIGN(4K)
    {
        __KERNEL_TEXT_START = .;
        *(.text.boot)
        *(.text.vector_table)
		*(.text)
        *(.text.*)
        __KERNEL_TEXT_END = .;
    }

    . = ALIGN(4);
    .init_array :
    {
        __init_array_start = .;
        KEEP (*(.init_array*))
        __init_array_end = .;
    }

    __KERNEL_IMAGE_END_ADDRESS = .;

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

    . = ALIGN(16K);
    __KERNEL_PAGE_DIRECTORY = .;
    . += 0x1000; /* Increment by one full page */

    . = ALIGN(4K);
    __KERNEL_STACK_BASE = .;
    . += 0x8000;
    __KERNEL_STACK_TOP = .;

    __ABORT_STACK_BASE = .;
    . += 0x1000;
    __ABORT_STACK_TOP = .;

    __UNDEFINED_STACK_BASE = .;
    . += 0x1000;
    __UNDEFINED_STACK_TOP = .;

    __IRQ_STACK_BASE = .;
    . += 0x1000;
    __IRQ_STACK_TOP = .;

    __KMALLOC_BASE = .;
    . += 4M;

    . = ALIGN(4);
    __FREE_RAM_START = .;

    . = (0x8000000 - .);
    __RAM_END = .;
}
