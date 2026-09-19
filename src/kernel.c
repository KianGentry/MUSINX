#include "riscv.h"

// goes one character at a time writing to console, making string
static void console_write(const char *text) {
    while (*text != '\0') {
        sbi_console_putchar(*text++);
    }
}

void kernel_main(void) {
    // sbi forward to debug console
    console_write("MUSINX\n");

    // sleep, wait for interrupt
    for (;;) {
        riscv_wfi();
    }
}