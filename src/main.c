#include "arch/riscv.h"
#include "trap.h"
#include "drivers/console.h"

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

// user mode test func
static void user(void) {
    __asm__ volatile(".word 0x00100073");

    console_write("user: returned from trap\n");

    for (;;) {
        wfi();
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