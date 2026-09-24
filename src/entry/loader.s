.section .text.entry

.globl _start
.type _start, @function

.extern kernel_main
.extern __kernel_stack_top

# set stack, jump to kernel_main
_start:
    # start at the top of stack
    la sp, __kernel_stack_top

    la t0, __kernel_stack_top
    csrw sscratch, t0

    # kernel.c
    call kernel_main

# if kernel_main returns
1:
    wfi
    # wait for interrupt in a loop
    j 1b

.size _start, . - _start