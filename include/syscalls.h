#ifndef SYSCALLS_H
#define SYSCALLS_H

#define SYSCALL_PUTCHAR 1

long syscall_dispatch(unsigned long number, unsigned long argument);

#endif // SYSCALLS_H