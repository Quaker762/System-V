ENTRY(_start)

SECTIONS {
    . = 0x80000000;
    __RAM_START = .;

    . = 0x80010000;
    __KERNEL_START = .;

    .text BLOCK(4K) : ALIGN(4K)
    {
        start_of_kernel_text = .;
        *(.text.boot)
        *(.text.vector_table)
		*(.text)
        *(.text.*)
        end_of_kernel_text = .;
    }

        . = ALIGN(4);
        .init_array :
        {
            __preinit_array_start = .;
            KEEP (*(.preinit_array*))
            __preinit_array_end = .;
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

    .= ALIGN(16K);
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
    . += 8M;

    . = ALIGN(16K);
    __L1_TABLE_POOL_START = .;
    . += 32M; /* 32MiB */
    __L1_TABLE_POOL_END = .;

    . = ALIGN(4K);

    __FREE_RAM_START = .;

    . = 0x100000000;
    __RAM_END = .;
}
