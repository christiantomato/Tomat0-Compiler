.data
fmt_int: .asciz "%d\n"
str0: .asciz "hiiiiii\n"

.text
.global _main

_voidFN:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//spill param to stack.
	str x0, [fp, #-8]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//print string.
	mov x0, x9
	bl _printf

	//load variable from stack.
	ldr x9, [fp, #-8]

	//print string.
	mov x0, x9
	bl _printf

	//move number to register.
	mov x9, #0

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

	//load string address from label.
	adrp x9, str0@PAGE
	add x9, x9, str0@PAGEOFF

	//pass parameter.
	mov x0, x9

	//call function.
	bl _voidFN

	//put status code 0 in ret register.
	mov x0, #0
	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

