#ifndef RISCV_H
#define RISCV_H

long sbi_console_put_char(int character);
void set_trap_vector(void (*handler)(void));
void trap_vector(void);
void wfi(void);

#endif // RISCV_H