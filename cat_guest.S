	.text
	.global _start
	.code16
_start:
loop:
	inb $0x30,%al	// al = getchar()
	movb %al,(buf)	// buf[0] = al
	movw $buf,%ax
	outw %ax,$0x10  // printf("%s\n", buf);
	jmp loop

	.data
buf:
	.ascii "\0\0"
