ENTRY(_start)

SECTIONS {

    . = 0x100000;
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

    .= 0x40000000;
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(.bss.COMMON)
    }

    . = ALIGN(4K);
    __RAM_BASE_ADDRESS = .;
    __KERNEL_STACK_BASE = .;
    . += 0x1000;
    __KERNEL_STACK_TOP = .;

    . = 0x44000000;
    __RAM_TOP_ADDRESS = .;
}