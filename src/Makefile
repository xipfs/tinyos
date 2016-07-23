C_SOURCES = $(wildcard kernel/*.c lib/*.c apps/*.c usr/*.c)
HEADERS = $(wildcard kernel/*.h lib/*.h apps/*.h usr/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o kernel/interrupt.o}

# Change this if your cross-compiler is somewhere else
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32

# First rule is run by default
tinyos.bin: boot/boot.bin kernel.bin
	cat $^ > tinyos.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

run: tinyos.bin
	qemu-system-i386 -k en-us -fda tinyos.bin

# Open the connection to qemu and load our kernel-object file with symbols
debug: tinyos.bin kernel.elf
	qemu-system-i386 -k en-us -s -fda tinyos.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o tinyos.bin *.elf
	rm -rf kernel/*.o boot/*.bin boot/*.o lib/*.o apps/*.o usr/*.o
