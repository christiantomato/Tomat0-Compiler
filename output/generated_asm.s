.data
fmt_int: .asciz "%d\n"
str0: .asciz "x is less than y\n"
str1: .asciz "x is not less than y\n"

.text
.global _main

_main:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//move number to register.
	mov x9, #1

	//store variable to stack.
	str x9, [fp, #-8]

	//move number to register.
	mov x9, #2

	//store variable to stack.
	str x9, [fp, #-16]

	//put status code 0 in ret register.
	mov x0, #0
	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

