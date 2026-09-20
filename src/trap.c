#include "arch/riscv.h"
#include "trap.h"
#include "drivers/console.h"
#include "syscalls.h"

void trap_handle(struct trap_frame *frame) {
    if (frame->scause == 8) {
        unsigned long number = frame->registers[17]; // a7 is the 18th register (index 17)
        unsigned long argument = frame->registers[10]; // a0 is the 11th register (index 10)

        frame->registers[10] = syscall_dispatch(number, argument);
        frame->sepc += 4; // advance past the ecall instruction
        return;
    }

    if (frame->scause == 3) {
        frame->sepc += 4; // advance past the ebreak instruction
        return;
    }

    trap_unhandled(frame->scause);
}

void trap_unhandled(unsigned long cause) {
    console_write("kernel: unexpected trap, scause: ");
    console_write_hex(cause);

    for (;;) {
        wfi();
    }
}