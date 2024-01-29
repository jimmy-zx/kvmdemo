CFLAGS+=-Wall -Wextra -Wpedantic

all: main guest.bin

main: main.o kvmutil.o

guest.o: guest.S
	as -32 guest.S -o guest.o

guest.bin: guest.o
	ld -m elf_i386 --oformat binary -N -e _start -Ttext 0x0 -o guest.bin guest.o
