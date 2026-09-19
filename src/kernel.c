#include "riscv.h"

// goes one character at a time writing to console, making string
static void console_write(const char *text) {
    while (*text != '\0') {
        sbi_console_put_char(*text++);
    }
}

static void console_write_hex(unsigned long value) {
    static const char hex_chars[] = "0123456789abcdef";
    console_write("0x");

    for (int shift = 60; shift >= 0; shift -= 4) {
        unsigned long digit = (value >> shift) & 0xf;
        sbi_console_put_char(hex_chars[digit]);
    }
    console_write("\n");
}

void trap_unhandled(unsigned long cause) {
    console_write("kernel: unexpected trap, scause: ");
    console_write_hex(cause);

    for (;;) {
        wfi();
    }
}

void kernel_main(void) {
    console_write("MUSINX\n");
    // sbi forward to debug console
    // set trap vector
    console_write("set trap\n");
    set_trap_vector(trap_vector);
    
    console_write("before trap\n");
    __asm__ volatile(".word 0x00100073"); // ebreak (4 byte)
    console_write("after trap\n");

    // sleep, wait for interrupt
    for (;;) {
        wfi();
    }
}