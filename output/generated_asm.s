.data
fmt_int: .asciz "%d\n"
str0: .asciz "aaahhh\n"
str1: .asciz "were okay don't worry\n"

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

	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #0

	//binary operation.
	cmp x9, x10
	cset x11, gt

	//check condition.
	cmp x11, #1
	//if not.
	bne _endif0

_if0:
	//load string address from label.
	adrp x9, str0@PAGE
	add x9, x9, str0@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//throw tomato - terminate runtime.
	mov x0, #1
	mov x16, #1
	svc #0x80

_endif0:
	//load string address from label.
	adrp x9, str1@PAGE
	add x9, x9, str1@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//put status code 0 in ret register.
	mov x0, #0
	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

