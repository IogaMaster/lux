OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS
{
    /* QEMU RISC-V Virt RAM starts here */
    . = 0x80000000;

    /* 1. Code: Make sure entry.S is at the very front */
    .text : {
        *(.text.entry)
        *(.text .text.*)
    }

    /* 2. Read-only data (constants, strings) */
    .rodata : {
        . = ALIGN(16);
        *(.rodata .rodata.*)
    }

    /* 3. Initialized data (globals with values) */
    .data : {
        . = ALIGN(16);
        *(.data .data.*)
    }

    /* 4. Uninitialized data (globals set to 0) */
    .bss : {
        . = ALIGN(16);
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        _bss_end = .;
    }

    /* Record the end of the kernel image */
    . = ALIGN(4096);
    _end = .;
}
