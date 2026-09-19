#include "arch/riscv.h"
#include "drivers/console.h"

// goes one character at a time writing to console, making string
void console_write(const char *text) {
    while (*text != '\0') {
        sbi_console_put_char(*text++);
    }
}

void console_write_hex(unsigned long value) {
    static const char hex_chars[] = "0123456789abcdef";
    console_write("0x");

    for (int shift = 60; shift >= 0; shift -= 4) {
        unsigned long digit = (value >> shift) & 0xf;
        sbi_console_put_char(hex_chars[digit]);
    }
    console_write("\n");
}