OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS
{
	. = 0x80200000;

	.text : ALIGN(4) {
		KEEP(*(.text.entry))
		*(.text .text.*)
	}

	.rodata : ALIGN(8) {
		*(.rodata .rodata.*)
	}

	.data : ALIGN(8) {
		*(.data .data.*)
	}

	.bss : ALIGN(8) {
		__bss_start = .;
		*(.bss .bss.* COMMON)
		__bss_end = .;
	}

	. = ALIGN(16);
	__stack_bottom = .;
	. += 16384;
	__stack_top = .;

	/DISCARD/ : {
		*(.eh_frame .comment)
		*(.note .note.*)
	}
}
