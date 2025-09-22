//setup
.text
.global _main
.balign 4

//entry point and stack setup
_main:
	sub sp, sp, #32
	stp x29, x30, [sp, #16]
	add x29, sp, #16

	//Code Gen Starts Here: 

	//move value to register
	mov x0, #1
	//store value to stack
	str x0, [x29, #-8]
	//move value to register
	mov x0, #4
	//store value to stack
	str x0, [x29, #-16]
	//move value to register
	mov x0, #2
	//load value from stack
	ldr x1, [x29, #-8]
	//binary operation
	add x2, x0, x1
	//load value from stack
	ldr x0, [x29, #-16]
	//binary operation
	add x1, x2, x0
	//store value to stack
	str x1, [x29, #-8]
	//load value from stack
	ldr x0, [x29, #-8]
	//print
	mov x8, x0
	adr x0, integerformatstr
	mov x1, x8
	str x1, [sp]
	bl _printf

	//exit and clean up
	mov x0, #0
	ldp x29, x30, [sp, #16]
	add sp, sp, #32
	ret

//data
integerformatstr:
	.asciz "%d\n"

stringformatstr:
	.asciz "%s\n"