.section .text
.align 2
.globl trap_vector
.type trap_vector, @function
.extern trap_unhandled
.extern syscall_dispatch

trap_vector:
    # save return address (for C)
    addi sp, sp, -32
    sd ra, 24(sp)
    sd a0, 8(sp)
    sd a7, 16(sp)

    # read trap reason (only scause 3)
    csrr a0, scause

    # user-mode ecall = SYSCALL!
    li t0, 8
    beq a0, t0, syscall_trap

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
    j return_from_trap

skip_four_bytes:
    addi t0, t0, 4
    j return_from_trap

syscall_trap:
    # load syscall number and arguments
    ld a1, 8(sp)
    ld a0, 16(sp)
    call syscall_dispatch

    # ecall is 4 bytes
    csrr t0, sepc
    addi t0, t0, 4

return_from_trap:
    csrw sepc, t0

    # return from breakpoint
    ld ra, 24(sp)
    addi sp, sp, 32
    sret

unexpected_trap:
    # pass to C
    call trap_unhandled

.size trap_vector, . - trap_vector
