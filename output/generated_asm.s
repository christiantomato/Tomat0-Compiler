.data
fmt_int: .asciz "%d\n"
str0: .asciz "sum is greater than 10\n"
str1: .asciz "sum is 10 or less\n"
str2: .asciz "x or y is not positive\n"

.text
.global _main

_main:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//move number to register.
	mov x9, #2

	//store variable to stack.
	str x9, [fp, #-8]

	//move number to register.
	mov x9, #4

	//store variable to stack.
	str x9, [fp, #-16]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #0

	//binary operation.
	cmp x9, x10
	cset x11, gt

	//load variable from stack.
	ldr x9, [fp, #-16]

	//move number to register.
	mov x10, #0

	//binary operation.
	cmp x9, x10
	cset x12, gt

	//binary operation.
	and x9, x11, x12

	cmp x9, #1
	beq _if0
	bne _elseforif0

_if0:
	//load variable from stack.
	ldr x10, [fp, #-8]

	//load variable from stack.
	ldr x11, [fp, #-16]

	//binary operation.
	add x12, x10, x11

	//store variable to stack.
	str x12, [fp, #-24]

	//load variable from stack.
	ldr x10, [fp, #-24]

	//move number to register.
	mov x11, #10

	//binary operation.
	cmp x10, x11
	cset x12, gt

	cmp x12, #1
	beq _if1
	bne _elseforif1

_if1:
	//load string address from label.
	adrp x10, str0@PAGE
	add x10, x10, str0@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	b _endif1

_elseforif1:
	//load string address from label.
	adrp x10, str1@PAGE
	add x10, x10, str1@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

_endif1:
	b _endif0

_elseforif0:
	//load string address from label.
	adrp x10, str2@PAGE
	add x10, x10, str2@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

_endif0:
	//put status code 0 in ret register.
	mov x0, #0
	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

