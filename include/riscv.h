#ifndef RISCV_H
#define RISCV_H

// ask SBI to output one character to console
static inline long sbi_console_putchar(int character) {
    register unsigned long a0 __asm__("a0") = (unsigned long)character;
    register unsigned long a7 __asm__("a7") = 1;

    // cross to SBI firmware
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a7) : "memory"); // unreadable slop (asm)
    return (long)a0;
}

// sleep, wait for interrupt
static inline void riscv_wfi(void) {
    __asm__ volatile("wfi" ::: "memory");
}

#endif // RISCV_H