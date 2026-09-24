#include "arch/riscv/riscv.h"
#include "trap.h"
#include "drivers/console.h"
#include "syscalls.h"

static void user_putchar(char character) {
    register unsigned long argument __asm__("a0") = character;
    register unsigned long number __asm__("a7") = SYSCALL_PUTCHAR;
    __asm__ volatile(
        "ecall" 
        : "+r"(argument)
        : "r"(number) 
        : "memory"
    );
}

static inline void enter_user_mode(void (*entry)(void)) {
    unsigned long status;
    extern char __kernel_stack_top[];
    extern char __stack_top[];

    __asm__ volatile("csrr %0, sstatus" : "=r"(status));

    status &= ~(1UL << 8); // clear SPP bit to enter user mode

    // set the trap vector to the user entry point
    __asm__ volatile(
        "csrw sscratch, %0\n"
        "mv sp, %1\n"
        "csrw sstatus, %2\n"
        "csrw sepc, %3\n"
        "sret"
        :
        : "r"(__kernel_stack_top), 
        "r"(__stack_top), 
        "r"(status), 
        "r"(entry)
        : "memory"
    );
    __builtin_unreachable();
}

// user mode test func
static void user(void) {

    user_putchar('H');
    user_putchar('i');
    user_putchar('\n');

    for (;;) {
    }
}

void kernel_main(void) {
    console_write("MUSINX\n");

    console_write("set trap\n");
    set_trap_vector(trap_vector);

    console_write("entering user mode\n");
    enter_user_mode(user);
    console_write("returned from user mode\n");

    // sleep, wait for interrupt
    for (;;) {
        wfi();
    }
}