ENTRY(_start)

SECTIONS {

    . = 0x80010000;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.text.boot)
        start_of_kernel_text = .;
		*(.text)
        end_of_kernel_text = .;
    }

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

    STACK_BASE = .;
    . += 0x100000;
    STACK_TOP = .;
}