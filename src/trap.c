#include "arch/riscv.h"
#include "trap.h"
#include "drivers/console.h"

void trap_unhandled(unsigned long cause) {
    console_write("kernel: unexpected trap, scause: ");
    console_write_hex(cause);

    for (;;) {
        wfi();
    }
}