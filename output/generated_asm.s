.data
fmt_int: .asciz "%d"
str0: .asciz "Welcome to Tomat0!\n\n"
str1: .asciz "     \\    |    /   \n"
str2: .asciz "       \\  |  /     \n"
str3: .asciz "         \\|/       \n"
str4: .asciz "^"
str5: .asciz "*"
str6: .asciz "*"
str7: .asciz " "
str8: .asciz "\n"
str9: .asciz "\nBelow is an example program which does vector computations.\n\n"
str10: .asciz "["
str11: .asciz ", "
str12: .asciz "]\n"
str13: .asciz "no square root found.\n"
str14: .asciz "vector 1: "
str15: .asciz "vector 2: "
str16: .asciz "vector 3: "
str17: .asciz "vector 4: "
str18: .asciz "magnitude of vector1: "
str19: .asciz "\n"
str20: .asciz "magnitude of vector2: "
str21: .asciz "\n"
str22: .asciz "magnitude of vector3: "
str23: .asciz "\n"
str24: .asciz "magnitude of vector4: "
str25: .asciz "\n"
str26: .asciz "vector 1 and vector 2 are orthogonal.\n"
str27: .asciz "vector 1 and 2 are not orthogonal, dot product was: "
str28: .asciz "\n"
str29: .asciz "vector 1 and vector 4 are orthogonal.\n"
str30: .asciz "vector 1 and vector 4 are not orthogonal, dot product was: "
str31: .asciz "\n"
str32: .asciz "vector 3 is a unit vector.\n"
str33: .asciz "vector 3 is not a unit vector, length is: "
str34: .asciz "\n"
str35: .asciz "vector 4 is a unit vector.\n"
str36: .asciz "vector 4 is not a unit vector, length is: "
str37: .asciz "\n"
str38: .asciz "component of v1 on v4 is: "
str39: .asciz "\n"
str40: .asciz "component of v4 on v3 is: "
str41: .asciz "\n"

.text
.global _main

_welcome:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

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

	//move number to register.
	mov x9, #0

	//store variable to stack.
	str x9, [fp, #-8]

_while0:
	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #7

	//binary operation.
	cmp x9, x10
	cset x9, lt

	//check condition.
	cmp x9, #1
	bne _loopend0

	//move number to register.
	mov x9, #0

	//store variable to stack.
	str x9, [fp, #-16]

_while1:
	//load variable from stack.
	ldr x9, [fp, #-16]

	//move number to register.
	mov x10, #21

	//binary operation.
	cmp x9, x10
	cset x9, lt

	//check condition.
	cmp x9, #1
	bne _loopend1

	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #3

	//binary operation.
	cmp x9, x10
	cset x9, eq

	//load variable from stack.
	ldr x10, [fp, #-16]

	//move number to register.
	mov x11, #8

	//binary operation.
	cmp x10, x11
	cset x10, eq

	//load variable from stack.
	ldr x11, [fp, #-16]

	//move number to register.
	mov x12, #12

	//binary operation.
	cmp x11, x12
	cset x11, eq

	//binary operation.
	orr x12, x10, x11

	//binary operation.
	and x10, x9, x12

	//check condition.
	cmp x10, #1
	//if.
	beq _if0
	//else.
	bne _elseforif0

_if0:
	//load string address from label.
	adrp x9, str4@PAGE
	add x9, x9, str4@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//load variable from stack.
	ldr x9, [fp, #-16]

	//move number to register.
	mov x10, #1

	//binary operation.
	add x11, x9, x10

	//store variable to stack.
	str x11, [fp, #-16]

	b _endif0

_elseforif0:
	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #0

	//binary operation.
	cmp x9, x10
	cset x9, eq

	//load variable from stack.
	ldr x10, [fp, #-8]

	//move number to register.
	mov x11, #6

	//binary operation.
	cmp x10, x11
	cset x10, eq

	//binary operation.
	orr x11, x9, x10

	//check condition.
	cmp x11, #1
	//if.
	beq _if1
	//else.
	bne _elseforif1

_if1:
	//load string address from label.
	adrp x9, str5@PAGE
	add x9, x9, str5@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//load variable from stack.
	ldr x9, [fp, #-16]

	//move number to register.
	mov x10, #1

	//binary operation.
	add x11, x9, x10

	//store variable to stack.
	str x11, [fp, #-16]

	b _endif1

_elseforif1:
	//load variable from stack.
	ldr x9, [fp, #-16]

	//move number to register.
	mov x10, #0

	//binary operation.
	cmp x9, x10
	cset x9, eq

	//load variable from stack.
	ldr x10, [fp, #-16]

	//move number to register.
	mov x11, #20

	//binary operation.
	cmp x10, x11
	cset x10, eq

	//binary operation.
	orr x11, x9, x10

	//check condition.
	cmp x11, #1
	//if.
	beq _if2
	//else.
	bne _elseforif2

_if2:
	//load string address from label.
	adrp x9, str6@PAGE
	add x9, x9, str6@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	b _endif2

_elseforif2:
	//load string address from label.
	adrp x9, str7@PAGE
	add x9, x9, str7@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

_endif2:
	//load variable from stack.
	ldr x9, [fp, #-16]

	//move number to register.
	mov x10, #1

	//binary operation.
	add x11, x9, x10

	//store variable to stack.
	str x11, [fp, #-16]

_endif1:
_endif0:
	//loop.
	b _while1

_loopend1:
	//load string address from label.
	adrp x9, str8@PAGE
	add x9, x9, str8@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #1

	//binary operation.
	add x11, x9, x10

	//store variable to stack.
	str x11, [fp, #-8]

	//loop.
	b _while0

_loopend0:
	//load string address from label.
	adrp x9, str9@PAGE
	add x9, x9, str9@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_vectorToString:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//spill param to stack.
	str x0, [fp, #-8]

	//spill param to stack.
	str x1, [fp, #-16]

	//load string address from label.
	adrp x9, str10@PAGE
	add x9, x9, str10@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//load variable from stack.
	ldr x9, [fp, #-8]

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x9, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x9, str11@PAGE
	add x9, x9, str11@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//load variable from stack.
	ldr x9, [fp, #-16]

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x9, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x9, str12@PAGE
	add x9, x9, str12@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_dot:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #32

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

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_isOrthogonal:
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

	//pass parameter.
	mov x0, x9

	//load variable from stack.
	ldr x9, [fp, #-16]

	//pass parameter.
	mov x1, x9

	//load variable from stack.
	ldr x9, [fp, #-24]

	//pass parameter.
	mov x2, x9

	//load variable from stack.
	ldr x9, [fp, #-32]

	//pass parameter.
	mov x3, x9

	//call function.
	bl _dot

	//move result into general register for safety.
	mov x9, x0

	//store variable to stack.
	str x9, [fp, #-40]

	//load variable from stack.
	ldr x9, [fp, #-40]

	//move number to register.
	mov x10, #0

	//binary operation.
	cmp x9, x10
	cset x9, eq

	//check condition.
	cmp x9, #1
	//if not.
	bne _endif3

_if3:
	//move number to register.
	mov x9, #1

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_endif3:
	//move number to register.
	mov x9, #0

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_squareRoot:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//spill param to stack.
	str x0, [fp, #-8]

	//move number to register.
	mov x9, #1

	//store variable to stack.
	str x9, [fp, #-16]

_while2:
	//load variable from stack.
	ldr x9, [fp, #-16]

	//load variable from stack.
	ldr x10, [fp, #-8]

	//binary operation.
	cmp x9, x10
	cset x9, lt

	//load variable from stack.
	ldr x10, [fp, #-16]

	//load variable from stack.
	ldr x11, [fp, #-8]

	//binary operation.
	cmp x10, x11
	cset x10, eq

	//binary operation.
	orr x11, x9, x10

	//check condition.
	cmp x11, #1
	bne _loopend2

	//load variable from stack.
	ldr x9, [fp, #-16]

	//load variable from stack.
	ldr x10, [fp, #-16]

	//binary operation.
	mul x11, x9, x10

	//load variable from stack.
	ldr x9, [fp, #-8]

	//binary operation.
	cmp x11, x9
	cset x9, eq

	//check condition.
	cmp x9, #1
	//if not.
	bne _endif4

_if4:
	//load variable from stack.
	ldr x9, [fp, #-16]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_endif4:
	//load variable from stack.
	ldr x9, [fp, #-16]

	//move number to register.
	mov x10, #1

	//binary operation.
	add x11, x9, x10

	//store variable to stack.
	str x11, [fp, #-16]

	//loop.
	b _while2

_loopend2:
	//load string address from label.
	adrp x9, str13@PAGE
	add x9, x9, str13@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//throw tomato - terminate runtime.
	mov x0, #1
	mov x16, #1
	svc #0x80

_magnitude:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #32

	//spill param to stack.
	str x0, [fp, #-8]

	//spill param to stack.
	str x1, [fp, #-16]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//load variable from stack.
	ldr x10, [fp, #-8]

	//binary operation.
	mul x11, x9, x10

	//load variable from stack.
	ldr x9, [fp, #-16]

	//load variable from stack.
	ldr x10, [fp, #-16]

	//binary operation.
	mul x12, x9, x10

	//binary operation.
	add x9, x11, x12

	//pass parameter.
	mov x0, x9

	//call function.
	bl _squareRoot

	//move result into general register for safety.
	mov x9, x0

	//store variable to stack.
	str x9, [fp, #-24]

	//load variable from stack.
	ldr x9, [fp, #-24]

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_isNormalized:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//spill param to stack.
	str x0, [fp, #-8]

	//spill param to stack.
	str x1, [fp, #-16]

	//load variable from stack.
	ldr x9, [fp, #-8]

	//pass parameter.
	mov x0, x9

	//load variable from stack.
	ldr x9, [fp, #-16]

	//pass parameter.
	mov x1, x9

	//call function.
	bl _magnitude

	//move result into general register for safety.
	mov x9, x0

	//move number to register.
	mov x10, #1

	//binary operation.
	cmp x9, x10
	cset x9, eq

	//check condition.
	cmp x9, #1
	//if not.
	bne _endif5

_if5:
	//move number to register.
	mov x9, #1

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_endif5:
	//move number to register.
	mov x9, #0

	//return into x0.
	mov x0, x9

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_comp:
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

	//pass parameter.
	mov x0, x9

	//load variable from stack.
	ldr x9, [fp, #-16]

	//pass parameter.
	mov x1, x9

	//load variable from stack.
	ldr x9, [fp, #-24]

	//pass parameter.
	mov x2, x9

	//load variable from stack.
	ldr x9, [fp, #-32]

	//pass parameter.
	mov x3, x9

	//call function.
	bl _dot

	//move result into general register for safety.
	mov x9, x0

	//store variable to stack.
	str x9, [fp, #-40]

	//load variable from stack.
	ldr x9, [fp, #-24]

	//pass parameter.
	mov x0, x9

	//load variable from stack.
	ldr x9, [fp, #-32]

	//pass parameter.
	mov x1, x9

	//call function.
	bl _magnitude

	//move result into general register for safety.
	mov x9, x0

	//store variable to stack.
	str x9, [fp, #-48]

	//load variable from stack.
	ldr x9, [fp, #-40]

	//load variable from stack.
	ldr x10, [fp, #-48]

	//binary operation.
	sdiv x11, x9, x10

	//return into x0.
	mov x0, x11

	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

_main:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #64

	//call function.
	bl _welcome

	//move number to register.
	mov x9, #3

	//store variable to stack.
	str x9, [fp, #-8]

	//move number to register.
	mov x9, #4

	//store variable to stack.
	str x9, [fp, #-16]

	//load string address from label.
	adrp x9, str14@PAGE
	add x9, x9, str14@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//load variable from stack.
	ldr x9, [fp, #-8]

	//pass parameter.
	mov x0, x9

	//load variable from stack.
	ldr x9, [fp, #-16]

	//pass parameter.
	mov x1, x9

	//call function.
	bl _vectorToString

	//move number to register.
	mov x9, #4

	//store variable to stack.
	str x9, [fp, #-24]

	//move number to register.
	mov x9, #3

	//integer negation.
	neg x10, x9

	//store variable to stack.
	str x10, [fp, #-32]

	//load string address from label.
	adrp x10, str15@PAGE
	add x10, x10, str15@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load variable from stack.
	ldr x10, [fp, #-24]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-32]

	//pass parameter.
	mov x1, x10

	//call function.
	bl _vectorToString

	//move number to register.
	mov x10, #1

	//store variable to stack.
	str x10, [fp, #-40]

	//move number to register.
	mov x10, #0

	//store variable to stack.
	str x10, [fp, #-48]

	//load string address from label.
	adrp x10, str16@PAGE
	add x10, x10, str16@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load variable from stack.
	ldr x10, [fp, #-40]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-48]

	//pass parameter.
	mov x1, x10

	//call function.
	bl _vectorToString

	//move number to register.
	mov x10, #0

	//store variable to stack.
	str x10, [fp, #-56]

	//move number to register.
	mov x10, #5

	//store variable to stack.
	str x10, [fp, #-64]

	//load string address from label.
	adrp x10, str17@PAGE
	add x10, x10, str17@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load variable from stack.
	ldr x10, [fp, #-56]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-64]

	//pass parameter.
	mov x1, x10

	//call function.
	bl _vectorToString

	//load string address from label.
	adrp x10, str18@PAGE
	add x10, x10, str18@PAGEOFF

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

	//call function.
	bl _magnitude

	//move result into general register for safety.
	mov x10, x0

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str19@PAGE
	add x10, x10, str19@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load string address from label.
	adrp x10, str20@PAGE
	add x10, x10, str20@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load variable from stack.
	ldr x10, [fp, #-24]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-32]

	//pass parameter.
	mov x1, x10

	//call function.
	bl _magnitude

	//move result into general register for safety.
	mov x10, x0

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str21@PAGE
	add x10, x10, str21@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load string address from label.
	adrp x10, str22@PAGE
	add x10, x10, str22@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load variable from stack.
	ldr x10, [fp, #-40]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-48]

	//pass parameter.
	mov x1, x10

	//call function.
	bl _magnitude

	//move result into general register for safety.
	mov x10, x0

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str23@PAGE
	add x10, x10, str23@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load string address from label.
	adrp x10, str24@PAGE
	add x10, x10, str24@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load variable from stack.
	ldr x10, [fp, #-56]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-64]

	//pass parameter.
	mov x1, x10

	//call function.
	bl _magnitude

	//move result into general register for safety.
	mov x10, x0

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str25@PAGE
	add x10, x10, str25@PAGEOFF

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
	bl _isOrthogonal

	//move result into general register for safety.
	mov x10, x0

	//check condition.
	cmp x10, #1
	//if.
	beq _if6
	//else.
	bne _elseforif6

_if6:
	//load string address from label.
	adrp x10, str26@PAGE
	add x10, x10, str26@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	b _endif6

_elseforif6:
	//load string address from label.
	adrp x10, str27@PAGE
	add x10, x10, str27@PAGEOFF

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

	//move result into general register for safety.
	mov x10, x0

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str28@PAGE
	add x10, x10, str28@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

_endif6:
	//load variable from stack.
	ldr x10, [fp, #-8]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-16]

	//pass parameter.
	mov x1, x10

	//load variable from stack.
	ldr x10, [fp, #-56]

	//pass parameter.
	mov x2, x10

	//load variable from stack.
	ldr x10, [fp, #-64]

	//pass parameter.
	mov x3, x10

	//call function.
	bl _isOrthogonal

	//move result into general register for safety.
	mov x10, x0

	//check condition.
	cmp x10, #1
	//if.
	beq _if7
	//else.
	bne _elseforif7

_if7:
	//load string address from label.
	adrp x10, str29@PAGE
	add x10, x10, str29@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	b _endif7

_elseforif7:
	//load string address from label.
	adrp x10, str30@PAGE
	add x10, x10, str30@PAGEOFF

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
	ldr x10, [fp, #-56]

	//pass parameter.
	mov x2, x10

	//load variable from stack.
	ldr x10, [fp, #-64]

	//pass parameter.
	mov x3, x10

	//call function.
	bl _dot

	//move result into general register for safety.
	mov x10, x0

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str31@PAGE
	add x10, x10, str31@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

_endif7:
	//load variable from stack.
	ldr x10, [fp, #-40]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-48]

	//pass parameter.
	mov x1, x10

	//call function.
	bl _isNormalized

	//move result into general register for safety.
	mov x10, x0

	//check condition.
	cmp x10, #1
	//if.
	beq _if8
	//else.
	bne _elseforif8

_if8:
	//load string address from label.
	adrp x10, str32@PAGE
	add x10, x10, str32@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	b _endif8

_elseforif8:
	//load string address from label.
	adrp x10, str33@PAGE
	add x10, x10, str33@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load variable from stack.
	ldr x10, [fp, #-40]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-48]

	//pass parameter.
	mov x1, x10

	//call function.
	bl _magnitude

	//move result into general register for safety.
	mov x10, x0

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str34@PAGE
	add x10, x10, str34@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

_endif8:
	//load variable from stack.
	ldr x10, [fp, #-56]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-64]

	//pass parameter.
	mov x1, x10

	//call function.
	bl _isNormalized

	//move result into general register for safety.
	mov x10, x0

	//check condition.
	cmp x10, #1
	//if.
	beq _if9
	//else.
	bne _elseforif9

_if9:
	//load string address from label.
	adrp x10, str35@PAGE
	add x10, x10, str35@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	b _endif9

_elseforif9:
	//load string address from label.
	adrp x10, str36@PAGE
	add x10, x10, str36@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load variable from stack.
	ldr x10, [fp, #-56]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-64]

	//pass parameter.
	mov x1, x10

	//call function.
	bl _magnitude

	//move result into general register for safety.
	mov x10, x0

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str37@PAGE
	add x10, x10, str37@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

_endif9:
	//load string address from label.
	adrp x10, str38@PAGE
	add x10, x10, str38@PAGEOFF

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
	ldr x10, [fp, #-56]

	//pass parameter.
	mov x2, x10

	//load variable from stack.
	ldr x10, [fp, #-64]

	//pass parameter.
	mov x3, x10

	//call function.
	bl _comp

	//move result into general register for safety.
	mov x10, x0

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str39@PAGE
	add x10, x10, str39@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load string address from label.
	adrp x10, str40@PAGE
	add x10, x10, str40@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//load variable from stack.
	ldr x10, [fp, #-56]

	//pass parameter.
	mov x0, x10

	//load variable from stack.
	ldr x10, [fp, #-64]

	//pass parameter.
	mov x1, x10

	//load variable from stack.
	ldr x10, [fp, #-40]

	//pass parameter.
	mov x2, x10

	//load variable from stack.
	ldr x10, [fp, #-48]

	//pass parameter.
	mov x3, x10

	//call function.
	bl _comp

	//move result into general register for safety.
	mov x10, x0

	//print integer.
	adrp x0, fmt_int@PAGE
	add x0, x0, fmt_int@PAGEOFF
	str x10, [sp, #-16]!
	bl _printf
	add sp, sp, #16

	//load string address from label.
	adrp x10, str41@PAGE
	add x10, x10, str41@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

	//put status code 0 in ret register.
	mov x0, #0
	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

