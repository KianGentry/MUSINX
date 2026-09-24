#include "syscalls.h"
#include "arch/riscv/riscv.h"

long syscall_dispatch(unsigned long number, unsigned long argument) {
    switch (number) {
        case SYSCALL_PUTCHAR:
            sbi_console_put_char((int)argument);
            return 0;
        default:
            return -1;
    }
}