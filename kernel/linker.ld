ENTRY(_start)

SECTIONS {

    . = 0x100000;

    .text BLOCK(4K) : ALIGN(4K)
    {
        start_of_kernel_text = .;
        *(.text.boot)
        *(.text.vector_table)
		*(.text)
        *(.text.*)
        end_of_kernel_text = .;
    }

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

    __RAM_BASE_ADDRESS = .;
    __STACK_BASE = .;
    . += 0x100000;
    __STACK_TOP = .;
}