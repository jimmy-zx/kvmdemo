CFLAGS+=-Wall -Wextra -Wpedantic -O0 -g -std=c11 -fno-stack-protector

all: main guest.bin cat_guest.bin

main: main.o kvmutil.o

cat_guest.o: cat_guest.S
	as -32 $< -o $@

guest.o: guest.S
	as -32 $< -o $@

%.bin: %.o
	ld -m elf_i386 --oformat binary -N -e _start -Ttext 0x0 -o $@ $<

.PHONY: run

run: all
	setarch `uname -m` -R ./main
