ARCH = riscv64
TARGET = $(ARCH)-unknown-elf
CC = clang
LD = ld.lld

CFLAGS = --target=$(TARGET) -march=rv64imac -mabi=lp64 -mcmodel=medany \
	-ffreestanding -fno-builtin -fno-stack-protector -nostdlib \
	-Wall -Wextra -Werror -O2 -Iinclude

ASFLAGS = --target=$(TARGET) -march=rv64imac -mabi=lp64
LDFLAGS = -m elf64lriscv -T linker.ld -nostdlib
OBJECTS = build/loader.o build/kernel.o

.PHONY: all clean

all: build/kernel.elf

build:
	mkdir -p $@

build/loader.o: src/entry/loader.s | build
	$(CC) $(ASFLAGS) -c $< -o $@

build/kernel.o: src/kernel.c include/riscv.h | build
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.elf: linker.ld $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

qemu:
	qemu-system-riscv64 -machine virt -nographic -kernel build/kernel.elf

clean:
	rm -rf build