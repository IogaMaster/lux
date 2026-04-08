#include "registers.h"

.section .text.entry
.global _start

_start:
    # 1. Setup the stack pointer
    la sp, _stack_top

    # 2. Clear the BSS section
    la t0, _bss_start
    la t1, _bss_end
    
clear_bss:
    bge t0, t1, jump_to_kernel
    sd zero, 0(t0)      # Store 8 bytes of zero
    addi t0, t0, 8
    j clear_bss

jump_to_kernel:
    # 3. Jump to your kernel's kmain
    call kmain

spin:
    wfi
    j spin
