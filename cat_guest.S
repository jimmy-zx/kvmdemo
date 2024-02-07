	.text
	.global _start
	.code16
_start:
loop:
	inb $0x30,%al
	movb %al,(buf)
	movw $buf,%ax
	outw %ax,$0x10
	jmp loop

	.data
buf:
	.ascii "\0\0"
