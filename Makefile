ARCH = riscv64
TARGET = $(ARCH)-unknown-elf

CC = clang
LD = ld.lld

CFLAGS = --target=$(TARGET) -march=rv64imac -mabi=lp64 -mcmodel=medany \
	-ffreestanding -fno-builtin -fno-stack-protector -nostdlib \
	-Wall -Wextra -Werror -O2 -Iinclude

ASFLAGS = --target=$(TARGET) -march=rv64imac -mabi=lp64
LDFLAGS = -m elf64lriscv -T linker.ld -nostdlib

OBJECTS = \
	build/loader.o \
	build/trap.o \
	build/main.o \
	build/trap-c.o \
	build/console.o \
	build/riscv.o \
	build/syscall.o

.PHONY: all clean qemu

all: build/kernel.elf

build:
	mkdir -p $@

build/loader.o: src/entry/loader.s | build
	$(CC) $(ASFLAGS) -c $< -o $@

build/trap.o: src/entry/trap.s | build
	$(CC) $(ASFLAGS) -c $< -o $@

build/main.o: src/main.c include/arch/riscv.h include/drivers/console.h include/trap.h | build
	$(CC) $(CFLAGS) -c $< -o $@

build/trap-c.o: src/trap.c include/arch/riscv.h include/drivers/console.h include/trap.h | build
	$(CC) $(CFLAGS) -c $< -o $@

build/console.o: src/drivers/console.c include/arch/riscv.h include/drivers/console.h | build
	$(CC) $(CFLAGS) -c $< -o $@

build/riscv.o: src/arch/riscv.c include/arch/riscv.h | build
	$(CC) $(CFLAGS) -c $< -o $@

build/syscall.o: src/syscalls.c include/arch/riscv.h include/syscalls.h | build
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.elf: linker.ld $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

qemu: build/kernel.elf
	qemu-system-riscv64 -machine virt -nographic -bios default -kernel $<

clean:
	rm -rf build