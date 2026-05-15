.data
fmt_int: .asciz "%d\n"
str0: .asciz ">:(\n"
str1: .asciz ":D\n"
str2: .asciz ":(\n"
str3: .asciz "the result was:\n"

.text
.global _main

_main:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #32

	//move number to register.
	mov x9, #1

	//store variable to stack.
	str x9, [fp, #-8]

	//move number to register.
	mov x9, #2

	//store variable to stack.
	str x9, [fp, #-16]

	//move number to register.
	mov x9, #3

	//store variable to stack.
	str x9, [fp, #-24]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #2

	//binary operation.
	mul x11, x9, x10

	//load variable from stack.
	ldr x9, [fp, #-16]

	//binary operation.
	sub x10, x11, x9

	//move number to register.
	mov x9, #33

	//load variable from stack.
	ldr x11, [fp, #-24]

	//binary operation.
	sdiv x12, x9, x11

	//binary operation.
	add x9, x10, x12

	//move number to register.
	mov x10, #1

	//binary operation.
	add x11, x9, x10

	//store variable to stack.
	str x11, [fp, #-32]

	//load string address from label.
	adrp x9, str0@PAGE
	add x9, x9, str0@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//load string address from label.
	adrp x9, str1@PAGE
	add x9, x9, str1@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//load string address from label.
	adrp x9, str2@PAGE
	add x9, x9, str2@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//load string address from label.
	adrp x9, str3@PAGE
	add x9, x9, str3@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//load variable from stack.
	ldr x9, [fp, #-32]

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
