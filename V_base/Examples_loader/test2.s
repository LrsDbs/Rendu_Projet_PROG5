	.cpu arm7tdmi
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"test2.c"
	.global	i
	.bss
	.align	2
	.type	i, %object
	.size	i, 4
i:
	.space	4
	.text
	.align	2
	.global	incremente
	.syntax unified
	.arm
	.fpu softvfp
	.type	incremente, %function
incremente:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	mov	r0, r3
	add	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
	.size	incremente, .-incremente
	.ident	"GCC: (15:6.3.1+svn253039-1build1) 6.3.1 20170620"
