#include <drivers/uart.h>

void kmain() {
  uart_puts("Hello from Lux OS!\n");

  while (1) {
    // Stay alive
  }
}
