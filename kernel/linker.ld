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

    __PMM_BITMAP = .;
    . += 0x1000;
    __FREE_RAM_START = .;

    . = 0x100000000;
    __RAM_END = .;
}