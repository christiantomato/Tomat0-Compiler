.data
fmt_int: .asciz "%d\n"
fmt_str: .asciz "%s\n"

.text
.global _main

_main:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//move number to register.
	mov x9, #5

	//store variable to stack.
	str x9, [fp, #-8]

	//move number to register.
	mov x9, #10

	//load variable from stack.
	ldr x10, [fp, #-8]

	//binary operation.
	add x11, x9, x10

	//store variable to stack.
	str x11, [fp, #-16]

	//load variable from stack.
	ldr x9, [fp, #-16]

	//load variable from stack.
	ldr x10, [fp, #-8]

	//binary operation.
	sub x11, x9, x10

	//store variable to stack.
	str x11, [fp, #-8]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//load variable from stack.
	ldr x10, [fp, #-8]

	//binary operation.
	mul x11, x9, x10

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x11, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//put status code 0 in ret register.
	mov x0, #0
	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret
