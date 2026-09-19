#include "arch/riscv.h"

// ask SBI to output one character to console
long sbi_console_put_char(int character) {
    register unsigned long a0 __asm__("a0") = (unsigned long)character;
    register unsigned long a7 __asm__("a7") = 1;

    // cross to SBI firmware
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a7) : "memory"); // unreadable slop (asm)
    return (long)a0;
}

// sleep, wait for interrupt
void wfi(void) {
    __asm__ volatile("wfi" ::: "memory");
}

void set_trap_vector(void (*handler)(void)) {
    __asm__ volatile("csrw stvec, %0" :: "r"(handler));
}