.data
fmt_int: .asciz "%d\n"
str0: .asciz "hi :3\n"

.text
.global _main

_complex:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #48

	//spill param to stack.
	str x0, [fp, #-8]

	//spill param to stack.
	str x1, [fp, #-16]

	//spill param to stack.
	str x2, [fp, #-24]

	//spill param to stack.
	str x3, [fp, #-32]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//load variable from stack.
	ldr x10, [fp, #-32]

	//load variable from stack.
	ldr x11, [fp, #-16]

	//binary operation.
	mul x12, x10, x11

	//binary operation.
	add x10, x9, x12

	//load variable from stack.
	ldr x9, [fp, #-24]

	//binary operation.
	add x11, x10, x9

	//move number to register.
	mov x9, #1

	//binary operation.
	sub x10, x11, x9

	//store variable to stack.
	str x10, [fp, #-40]

	//load variable from stack.
	ldr x9, [fp, #-40]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_composition:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #48

	//spill param to stack.
	str x0, [fp, #-8]

	//spill param to stack.
	str x1, [fp, #-16]

	//spill param to stack.
	str x2, [fp, #-24]

	//spill param to stack.
	str x3, [fp, #-32]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//print string.
	mov x0, x9
	bl _printf

	//load variable from stack.
	ldr x9, [fp, #-16]

	//pass parameter.
	mov x0, x9

	//load variable from stack.
	ldr x9, [fp, #-24]

	//pass parameter.
	mov x1, x9

	//load variable from stack.
	ldr x9, [fp, #-32]

	//pass parameter.
	mov x2, x9

	//move number to register.
	mov x9, #10

	//pass parameter.
	mov x3, x9

	//call function.
	bl _complex

	//store variable to stack.
	str x0, [fp, #-40]

	//load variable from stack.
	ldr x9, [fp, #-40]

	//negate value.
	neg x10, x9

	//return into x0.
	mov x0, x10

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_main:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//load string address from label.
	adrp x10, str0@PAGE
	add x10, x10, str0@PAGEOFF

	//pass parameter.
	mov x0, x10

	//move number to register.
	mov x10, #1

	//pass parameter.
	mov x1, x10

	//move number to register.
	mov x10, #2

	//pass parameter.
	mov x2, x10

	//move number to register.
	mov x10, #3

	//pass parameter.
	mov x3, x10

	//call function.
	bl _composition

	//store variable to stack.
	str x0, [fp, #-8]

	//load variable from stack.
	ldr x10, [fp, #-8]

	//ensure int to print doesn't get clobbered.
	mov x11, x10
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

