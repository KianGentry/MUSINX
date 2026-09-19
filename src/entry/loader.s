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