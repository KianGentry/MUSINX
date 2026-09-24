#include "arch/riscv/riscv.h"
#include "trap.h"
#include "drivers/console.h"
#include "syscalls.h"
#include "arch/riscv/paging.h"

#define USER_TEXT __attribute__((section(".user.text")))

static USER_TEXT void user_putchar(char character) {
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
    extern char __user_stack_top[];

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
        "r"(__user_stack_top), 
        "r"(status), 
        "r"(entry)
        : "memory"
    );
    __builtin_unreachable();
}

// user mode test func
static USER_TEXT void user(void) {

    user_putchar('H');
    user_putchar('i');
    user_putchar('\n');

    for (;;) {
    }
}

void kernel_main(void) {
    extern char __kernel_stack_top[];
    console_write("MUSINX\n");

    console_write("set trap\n");
    set_trap_vector(trap_vector);

    // temp sscratch initialisation
    __asm__ volatile("csrw sscratch, %0" : : "r"(__kernel_stack_top) : "memory");

    console_write("start paging\n");
    paging_init();

    console_write("entering user mode\n");
    enter_user_mode(user);
    console_write("returned from user mode\n");

    // sleep, wait for interrupt
    for (;;) {
        wfi();
    }
}