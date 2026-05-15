.data
fmt_int: .asciz "%d\n"
str0: .asciz "u suck\n"
str1: .asciz "i like u\n"
str2: .asciz "hello world\n"

.text
.global _main

_main:
	//setup stack frame.
	stp fp, lr, [sp, #-16]!
	mov fp, sp
	sub sp, sp, #0

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

	//put status code 0 in ret register.
	mov x0, #0
	//collapse the stack frame.
	mov sp, fp
	ldp fp, lr, [sp], #16
	ret
