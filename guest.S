	.text
	.global _start
	.code16
_start:
	xorw %ax,%ax
loop:
	movw $target,%ax
	outw %ax,$0x10
	hlt

	.data
target:
	.ascii "Hello, world!\0"
