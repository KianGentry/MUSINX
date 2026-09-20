#ifndef TRAP_H
#define TRAP_H

struct trap_frame {
    unsigned long registers[32];
    unsigned long sepc;
    unsigned long scause;
    unsigned long stval;
};

void trap_handle(struct trap_frame *frame);
void trap_unhandled(unsigned long cause);

#endif // TRAP_H