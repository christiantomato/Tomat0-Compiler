.data
fmt_int: .asciz "%d\n"
fmt_str: .asciz "%s\n"

.text
.global _main

_main:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #0

	//store variable to stack.
	str x0, [fp, #24608]

	//store variable to stack.
	str x0, [fp, #24608]

	//put status code 0 in ret register.
	mov x0, #0
	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret
