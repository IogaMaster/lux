#include "memory/page_allocator.h"
#include "types.h"
#include <drivers/uart.h>

extern char _end[];
static uintptr_t mem_end = (uintptr_t)_end + (128 << 20); // kernel_end + 128MB of free mem

void kmain(void) {
    uart_puts("Lux OS booting....");
    pmm_init(mem_end);
    uart_puts("DONE\n");

    while (true) {
        // Stay alive
    }
}
