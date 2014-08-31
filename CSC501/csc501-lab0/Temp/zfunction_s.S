	.file	"zfunction_s.c"
	.section	.rodata
.LC0:
	.string	"Task 1 (zfunction)"
.LC1:
	.string	"\n Input: 0x%08x"
.LC2:
	.string	"\n Output: 0x%08x"
	.text
.globl zfunction_s
	.type	zfunction_s, @function
zfunction_s:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	$0, -4(%ebp)
	subl	$12, %esp
	pushl	$.LC0
	call	kprintf
	addl	$16, %esp
	movl	$-8372225, -8(%ebp)
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	$.LC1
	call	kprintf
	addl	$16, %esp
	movl	-8(%ebp), %eax
	andl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	leal	-4(%ebp), %eax
	sall	$4, (%eax)
	subl	$8, %esp
	pushl	-4(%ebp)
	pushl	$.LC2
	call	kprintf
	addl	$16, %esp
	movl	-4(%ebp), %eax
	leave
	ret
	.size	zfunction_s, .-zfunction_s
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.6 20060404 (Red Hat 3.4.6-3)"
