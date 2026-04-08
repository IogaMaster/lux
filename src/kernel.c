#include "types.h"
#include <drivers/uart.h>

void kmain(void) {
    uart_puts("Hello from Lux OS!\n");

    while (true) {
        // Stay alive
    }
}
