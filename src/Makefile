C_SOURCES = $(wildcard kernel/*.c cpu/*.c lib/*.c)
HEADERS = $(wildcard kernel/*.h cpu/*.h lib/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32

tinyos.bin: boot/boot.bin kernel.bin
	cat $^ > tinyos.bin

kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

run: tinyos.bin
	qemu-system-i386 -k en-us -fda tinyos.bin

debug: tinyos.bin kernel.elf
	qemu-system-i386 -s -fda tinyos.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o tinyos.bin *.elf
	rm -rf kernel/*.o boot/*.bin boot/*.o cpu/*.o lib/*.o
