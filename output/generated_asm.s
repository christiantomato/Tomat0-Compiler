//setup
.text
.global _main
.balign 4

//entry point and stack setup
_main:
	sub sp, sp, #32
	stp x29, x30, [sp, #8]
	add x29, sp, #8

	//Code Gen Starts Here: 

	//move value to register
	mov x0, #1
	//store value to stack
	str x0, [x29, #-8]

	//exit and clean up
	mov x0, #0
	ldp x29, x30, [sp, #8]
	add sp, sp, #32
	ret

//data
integerformatstr:
	.asciz "%d\n"

stringformatstr:
	.asciz "%s\n"