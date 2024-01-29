	.text
	.global _start
	.code16
_start:
	movw $target,%ax
	outw %ax,$0x10
	hlt

	.data
target:
	.ascii "Hello, world!\0"
