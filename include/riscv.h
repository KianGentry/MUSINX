#ifndef RISCV_H
#define RISCV_H

// enter user function
static inline void enter_user_mode(void (*entry)(void)) {
    unsigned long status;

    __asm__ volatile("csrr %0, sstatus" : "=r"(status));

    status &= ~(1UL << 8); // clear SPP bit to enter user mode

    // set the trap vector to the user entry point
    __asm__ volatile(
        "csrw sstatus, %0\n"
        "csrw sepc, %1\n"
        "sret"
        :
        : "r"(status), "r"(entry)
        : "memory"
    );
}

// ask SBI to output one character to console
static inline long sbi_console_put_char(int character) {
    register unsigned long a0 __asm__("a0") = (unsigned long)character;
    register unsigned long a7 __asm__("a7") = 1;

    // cross to SBI firmware
    __asm__ volatile("ecall" : "+r"(a0) : "r"(a7) : "memory"); // unreadable slop (asm)
    return (long)a0;
}

static inline void set_trap_vector(void (*handler)(void)) {
    __asm__ volatile("csrw stvec, %0" :: "r"(handler));
}

void trap_vector(void);

// sleep, wait for interrupt
static inline void wfi(void) {
    __asm__ volatile("wfi" ::: "memory");
}

#endif // RISCV_H