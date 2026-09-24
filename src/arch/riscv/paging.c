#include "arch/riscv/paging.h"
#include "drivers/console.h"

#define PTE_V (1UL << 0)
#define PTE_R (1UL << 1)
#define PTE_W (1UL << 2)
#define PTE_X (1UL << 3)
#define PTE_U (1UL << 4)
#define PTE_A (1UL << 6)
#define PTE_D (1UL << 7)

#define SATP_MODE_SV39 (8UL << 60)
#define KERNEL_BASE 0x80200000UL
#define USER_TEXT_BASE 0x80400000UL
#define USER_STACK_BASE 0x80600000UL
#define TWO_MIB 0x200000UL

static unsigned long root_table[512] __attribute__((aligned(4096)));
static unsigned long level_one[512] __attribute__((aligned(4096)));

static unsigned long page_entry(unsigned long address) {
    return (address >> 12) << 10;
}

static unsigned long table_entry(unsigned long *table) {
    return ((unsigned long)table >> 12) << 10;
}

static inline void write_satp(unsigned long value) {
    console_write("about to write satp\n");

    __asm__ volatile("csrw satp, %0\n"
        "sfence.vma\n"
                    :
                    : "r"(value)
                    : "memory");
}

void paging_init(void) {
    const unsigned long kernel_perms = PTE_V | PTE_R | PTE_W | PTE_X | PTE_A | PTE_D;
    const unsigned long user_text_perms = PTE_V | PTE_R | PTE_X | PTE_U | PTE_A;
    const unsigned long user_stack_perms = PTE_V | PTE_R | PTE_W | PTE_U | PTE_A | PTE_D;

    // 0x80000000 in slot 2 in root table
    // each level one entry maps 2mib memory
    root_table[2] = table_entry(level_one) | PTE_V;
    level_one[1] = page_entry(KERNEL_BASE) | kernel_perms;
    level_one[2] = page_entry(USER_TEXT_BASE) | user_text_perms;
    level_one[3] = page_entry(USER_STACK_BASE) | user_stack_perms;

    console_write("paging tables set up\n");
    unsigned long root = (unsigned long)root_table;

    console_write("root[2]: ");
    console_write_hex(root_table[2]);
    console_write("root: ");
    console_write_hex(root);
    write_satp(SATP_MODE_SV39 | (root >> 12));
    console_write("satp written\n");
}