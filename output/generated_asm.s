.data
fmt_int: .asciz "%d\n"
str0: .asciz "hello world\n"
str1: .asciz "ooga\n"

.text
.global _main

_main:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//move number to register.
	mov x9, #0

	//store variable to stack.
	str x9, [fp, #-8]

_while0:
	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #8

	//binary operation.
	cmp x9, x10
	cset x11, lt

	//check condition.
	cmp x11, #1
	bne _loopend0

	//move number to register.
	mov x9, #0

	//store variable to stack.
	str x9, [fp, #-16]

	//load string address from label.
	adrp x9, str0@PAGE
	add x9, x9, str0@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

_while1:
	//load variable from stack.
	ldr x9, [fp, #-16]

	//move number to register.
	mov x10, #3

	//binary operation.
	cmp x9, x10
	cset x12, lt

	//check condition.
	cmp x12, #1
	bne _loopend1

	//load string address from label.
	adrp x9, str1@PAGE
	add x9, x9, str1@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//load variable from stack.
	ldr x9, [fp, #-16]

	//move number to register.
	mov x10, #1

	//binary operation.
	add x13, x9, x10

	//store variable to stack.
	str x13, [fp, #-16]

	//loop.
	b _while1

_loopend1:
	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #1

	//binary operation.
	add x13, x9, x10

	//store variable to stack.
	str x13, [fp, #-8]

	//loop.
	b _while0

_loopend0:
	//put status code 0 in ret register.
	mov x0, #0
	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

