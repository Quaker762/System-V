ENTRY(_start)

SECTIONS {
    . = 0x00000000;
    __PHYSICAL_MEMORY_START = .;

    .text.vector_table BLOCK(4K) : ALIGN(4K)
    {
        *(.text.vector_table)
    }

    /**
     *  Put the kernel boot page directory
     *  at the first possible 16KiB aligned
     *  address.
     *
     *  It'd be nicer if this was in `start.s` but eh,
     *  whatever
     */
    . = 0x00004000;
    boot_page_directory = .;

    /**
     *  Boot section needs to be linked to
     *  the actual address that the processor
     *  starts us at!
     */
    . = 0x00010000;
    .text.boot BLOCK(4K) : ALIGN(4K)
    {
        *(.text.boot)
    }

    . = ALIGN(16K);
    __FREE_RAM_START = .;

    . = 0x8000000;
    __FREE_RAM_END = .;
    __PHYSICAL_MEMORY_END = .;

    /**
     *  The actual kernel image itself is linked
     *  at the 3GiB mark in the virtual address
     *  space
     */
    . = 0xC0100000;
    __KERNEL_IMAGE_START_ADDRESS = .;

    .text BLOCK(4K) : AT (ADDR (.text) - 0xC0000000)
    {
        __KERNEL_TEXT_START = .;
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

    .rodata BLOCK(4K) : AT (ADDR (.rodata) - 0xC0000000)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : AT (ADDR (.data) - 0xC0000000)
    {
        *(.data)
    }

    .bss BLOCK(4K) : AT (ADDR (.bss) - 0xC0000000)
    {
        *(.bss.COMMON)
    }

    __KERNEL_IMAGE_END_ADDRESS = .;

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
}
