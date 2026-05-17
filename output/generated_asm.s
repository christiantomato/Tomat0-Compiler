.data
fmt_int: .asciz "%d\n"

.text
.global _main

_add:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #32

	//spill param to stack.
	str x0, [fp, #-8]

	//spill param to stack.
	str x1, [fp, #-16]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//load variable from stack.
	ldr x10, [fp, #-16]

	//binary operation.
	add x11, x9, x10

	//store variable to stack.
	str x11, [fp, #-24]

	//load variable from stack.
	ldr x9, [fp, #-24]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_compose:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #32

	//spill param to stack.
	str x0, [fp, #-8]

	//spill param to stack.
	str x1, [fp, #-16]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//pass parameter.
	mov x0, x9

	//load variable from stack.
	ldr x9, [fp, #-16]

	//pass parameter.
	mov x1, x9

	//call function.
	bl _add

	//move number to register.
	mov x9, #1

	//binary operation.
	add x10, x0, x9

	//store variable to stack.
	str x10, [fp, #-24]

	//load variable from stack.
	ldr x9, [fp, #-24]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_main:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #0

	//move number to register.
	mov x9, #1

	//pass parameter.
	mov x0, x9

	//move number to register.
	mov x9, #2

	//pass parameter.
	mov x1, x9

	//call function.
	bl _compose

	//ensure int to print doesn't get clobbered.
	mov x9, x0
	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x9, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//put status code 0 in ret register.
	mov x0, #0
	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

