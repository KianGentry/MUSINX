.section .text.entry

.globl _start
.type _start, @function

.extern kernel_main
.extern __stack_top

# set stack, jump to kernel_main
_start:
    # start at the top of stack
    la sp, __stack_top

    # kernel.c
    call kernel_main

# if kernel_main returns
1:
    wfi
    # wait for interrupt in a loop
    j 1b

.size _start, . - _start

.section .text
.align 2
.globl trap_vector
.type trap_vector, @function
.extern trap_unhandled

trap_vector:
    # save return address (for C)
    addi sp, sp, -16
    sd ra, 8(sp)

    # read trap reason
    csrr a0, scause

    # unexpected trap handling, cause 3
    li t0, 3
    bne a0, t0, unexpected_trap

    # if ebreak, skip 4 bytes
    csrr t0, sepc
    addi t0, t0, 4
    csrw sepc, t0

    # return from (only) ebreak
    ld ra, 8(sp)
    addi sp, sp, 16
    sret

unexpected_trap:
    # pass to C
    call trap_unhandled

.size trap_vector, . - trap_vector
