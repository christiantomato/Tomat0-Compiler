.data
fmt_int: .asciz "%d\n"
str0: .asciz "calculating grade...\n"
str1: .asciz "invalid grade\n"
str2: .asciz "you did awesome!\n"
str3: .asciz "you did well\n"
str4: .asciz "at least you passed\n"
str5: .asciz "FAIL!!!\n"

.text
.global _main

_main:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #16

	//move number to register.
	mov x9, #90

	//store variable to stack.
	str x9, [fp, #-8]

	//load string address from label.
	adrp x9, str0@PAGE
	add x9, x9, str0@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #0

	//binary operation.
	cmp x9, x10
	cset x9, lt

	//load variable from stack.
	ldr x10, [fp, #-8]

	//move number to register.
	mov x11, #100

	//binary operation.
	cmp x10, x11
	cset x10, gt

	//binary operation.
	orr x11, x9, x10

	//check condition.
	cmp x11, #1
	//if.
	beq _if0
	//else.
	bne _elseforif0

_if0:
	//load string address from label.
	adrp x9, str1@PAGE
	add x9, x9, str1@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	//throw tomato - terminate runtime.
	mov x0, #1
	mov x16, #1
	svc #0x80

	b _endif0

_elseforif0:
	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #90

	//binary operation.
	cmp x9, x10
	cset x9, eq

	//load variable from stack.
	ldr x10, [fp, #-8]

	//move number to register.
	mov x12, #90

	//binary operation.
	cmp x10, x12
	cset x10, gt

	//binary operation.
	orr x12, x9, x10

	//check condition.
	cmp x12, #1
	//if.
	beq _if1
	//else.
	bne _elseforif1

_if1:
	//load string address from label.
	adrp x9, str2@PAGE
	add x9, x9, str2@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	b _endif1

_elseforif1:
	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #70

	//binary operation.
	cmp x9, x10
	cset x9, eq

	//load variable from stack.
	ldr x10, [fp, #-8]

	//move number to register.
	mov x13, #70

	//binary operation.
	cmp x10, x13
	cset x10, gt

	//binary operation.
	orr x13, x9, x10

	//check condition.
	cmp x13, #1
	//if.
	beq _if2
	//else.
	bne _elseforif2

_if2:
	//load string address from label.
	adrp x9, str3@PAGE
	add x9, x9, str3@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	b _endif2

_elseforif2:
	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #50

	//binary operation.
	cmp x9, x10
	cset x9, eq

	//load variable from stack.
	ldr x10, [fp, #-8]

	//move number to register.
	mov x14, #50

	//binary operation.
	cmp x10, x14
	cset x10, gt

	//binary operation.
	orr x14, x9, x10

	//check condition.
	cmp x14, #1
	//if.
	beq _if3
	//else.
	bne _elseforif3

_if3:
	//load string address from label.
	adrp x9, str4@PAGE
	add x9, x9, str4@PAGEOFF

	//print string.
	mov x0, x9
	bl _printf

	b _endif3

_elseforif3:
	//load variable from stack.
	ldr x9, [fp, #-8]

	//move number to register.
	mov x10, #50

	//binary operation.
	cmp x9, x10
	cset x9, lt

	//check condition.
	cmp x9, #1
	//if not.
	bne _endif4

_if4:
	//load string address from label.
	adrp x10, str5@PAGE
	add x10, x10, str5@PAGEOFF

	//print string.
	mov x0, x10
	bl _printf

_endif4:
_endif3:
_endif2:
_endif1:
_endif0:
	//put status code 0 in ret register.
	mov x0, #0
	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret

