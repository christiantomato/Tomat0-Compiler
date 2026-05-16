.data
fmt_int: .asciz "%d\n"
str0: .asciz "the answer is:\n"

.text
.global _main

_add:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//binary operation.
	add x9, x0, x1

	//store variable to stack.
	str x9, [fp, #-8]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_double:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #0

	//move number to register.
	mov x9, #2

	//binary operation.
	mul x10, x0, x9

	//moving to param reg.
	mov x0, x10

	//return into x0.
	mov x0, x0

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_main:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #0

	//load string address from label.
	adrp x9, str0@PAGE
	add x9, x9, str0@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//move number to register.
	mov x9, #1

	//pass parameter.
	mov x0, x9

	//move number to register.
	mov x9, #2

	//pass parameter.
	mov x1, x9

	//call function.
	bl _add

	//pass parameter.
	mov x0, x0

	//call function.
	bl _double

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

