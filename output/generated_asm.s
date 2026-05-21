.data
fmt_int: .asciz "%d\n"

.text
.global _main

_recursiveFactorial:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #32

	//spill param to stack.
	str x0, [fp, #-8]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #1

	//binary operation.
	cmp x9, x10
	cset x11, eq

	//check condition.
	cmp x11, #1
	//if not.
	bne _endif0

_if0:
	//load variable from stack.
	ldr x9, [fp, #-8]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_endif0:
	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #1

	//binary operation.
	sub x12, x9, x10

	//store variable to stack.
	str x12, [fp, #-16]

	//load variable from stack.
	ldr x9, [fp, #-16]

	//pass parameter.
	mov x0, x9

	//call function.
	bl _recursiveFactorial

	//store variable to stack.
	str x0, [fp, #-24]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//load variable from stack.
	ldr x10, [fp, #-24]

	//binary operation.
	mul x12, x9, x10

	//store variable to stack.
	str x12, [fp, #-8]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_recursiveFactorialBad:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #32

	//spill param to stack.
	str x0, [fp, #-8]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #1

	//binary operation.
	cmp x9, x10
	cset x12, eq

	//check condition.
	cmp x12, #1
	//if not.
	bne _endif1

_if1:
	//load variable from stack.
	ldr x9, [fp, #-8]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_endif1:
	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #1

	//binary operation.
	sub x13, x9, x10

	//store variable to stack.
	str x13, [fp, #-16]

	//load variable from stack.
	ldr x9, [fp, #-16]

	//pass parameter.
	mov x0, x9

	//call function.
	bl _recursiveFactorial

	//store variable to stack.
	str x0, [fp, #-24]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//load variable from stack.
	ldr x10, [fp, #-24]

	//binary operation.
	mul x13, x9, x10

	//store variable to stack.
	str x13, [fp, #-8]

	//load variable from stack.
	ldr x9, [fp, #-8]

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
	sub sp, sp, #16

	//move number to register.
	mov x9, #5

	//store variable to stack.
	str x9, [fp, #-8]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//pass parameter.
	mov x0, x9

	//call function.
	bl _recursiveFactorial

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

