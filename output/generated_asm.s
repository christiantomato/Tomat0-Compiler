.data
fmt_int: .asciz "%d\n"
str0: .asciz "doubling xComp0\n"
str1: .asciz "squaring xComp0\n"
str2: .asciz "dotting vectors...\n"
str3: .asciz "computing slope...\n"
str4: .asciz "lol :P\n"

.text
.global _main

_printThree:
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

_double:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//spill param to stack.
	str x0, [fp, #-8]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #2

	//binary operation.
	mul x11, x9, x10

	//store variable to stack.
	str x11, [fp, #-8]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_square:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//spill param to stack.
	str x0, [fp, #-8]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//load variable from stack.
	ldr x10, [fp, #-8]

	//binary operation.
	mul x11, x9, x10

	//store variable to stack.
	str x11, [fp, #-8]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_dot:
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
	ldr x10, [fp, #-24]

	//binary operation.
	mul x11, x9, x10

	//load variable from stack.
	ldr x9, [fp, #-16]

	//load variable from stack.
	ldr x10, [fp, #-32]

	//binary operation.
	mul x12, x9, x10

	//binary operation.
	add x9, x11, x12

	//store variable to stack.
	str x9, [fp, #-40]

	//load variable from stack.
	ldr x9, [fp, #-40]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_slope:
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
	ldr x9, [fp, #-32]

	//load variable from stack.
	ldr x10, [fp, #-16]

	//binary operation.
	sub x11, x9, x10

	//load variable from stack.
	ldr x9, [fp, #-24]

	//load variable from stack.
	ldr x10, [fp, #-8]

	//binary operation.
	sub x12, x9, x10

	//binary operation.
	sdiv x9, x11, x12

	//store variable to stack.
	str x9, [fp, #-40]

	//load variable from stack.
	ldr x9, [fp, #-40]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_complex:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #0

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
	sub sp, sp, #32

	//move number to register.
	mov x9, #3

	//store variable to stack.
	str x9, [fp, #-8]

	//move number to register.
	mov x9, #4

	//store variable to stack.
	str x9, [fp, #-16]

	//move number to register.
	mov x9, #4

	//negate value.
	neg x10, x9

	//store variable to stack.
	str x10, [fp, #-24]

	//move number to register.
	mov x10, #3

	//store variable to stack.
	str x10, [fp, #-32]

	//load string address from label.
	adrp x10, str0@PAGE
	add x10, x10, str0@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load variable from stack.
	ldr x10, [fp, #-8]

	//pass parameter.
	mov x0, x10

	//call function.
	bl _double

	//ensure int to print doesn't get clobbered.
	mov x10, x0
	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str1@PAGE
	add x10, x10, str1@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load variable from stack.
	ldr x10, [fp, #-8]

	//pass parameter.
	mov x0, x10

	//call function.
	bl _square

	//ensure int to print doesn't get clobbered.
	mov x10, x0
	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str2@PAGE
	add x10, x10, str2@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load variable from stack.
	ldr x10, [fp, #-8]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-16]

	//pass parameter.
	mov x1, x10

	//load variable from stack.
	ldr x10, [fp, #-24]

	//pass parameter.
	mov x2, x10

	//load variable from stack.
	ldr x10, [fp, #-32]

	//pass parameter.
	mov x3, x10

	//call function.
	bl _dot

	//ensure int to print doesn't get clobbered.
	mov x10, x0
	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str3@PAGE
	add x10, x10, str3@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//move number to register.
	mov x10, #2

	//pass parameter.
	mov x0, x10

	//move number to register.
	mov x10, #2

	//pass parameter.
	mov x1, x10

	//move number to register.
	mov x10, #0

	//pass parameter.
	mov x2, x10

	//move number to register.
	mov x10, #0

	//pass parameter.
	mov x3, x10

	//call function.
	bl _slope

	//ensure int to print doesn't get clobbered.
	mov x10, x0
	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str4@PAGE
	add x10, x10, str4@PAGEOFF

	//pass parameter.
	mov x0, x10

	//call function.
	bl _printThree

	//put status code 0 in ret register.
	mov x0, #0
	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

