.section .text
.align 2
.globl trap_vector
.type trap_vector, @function
.extern trap_unhandled

trap_vector:
    # save return address (for C)
    addi sp, sp, -16
    sd ra, 8(sp)

    # read trap reason (only scause 3)
    csrr a0, scause

    # unexpected trap handling
    li t0, 3
    bne a0, t0, unexpected_trap

    # read trapped instruction (address in sepc)
    csrr t0, sepc
    lhu t1, 0(t0)

    # if lowest 2 bits are 3, its 32 bit
    andi t1, t1, 3
    li t2, 3
    beq t1, t2, skip_four_bytes

    # else its 16 bit instruction
    addi t0, t0, 2
    j save_return_address

skip_four_bytes:
    addi t0, t0, 4

save_return_address:
    csrw sepc, t0

    # return from breakpoint
    ld ra, 8(sp)
    addi sp, sp, 16
    sret

unexpected_trap:
    # pass to C
    call trap_unhandled

.size trap_vector, . - trap_vector
