	.file	"zfunction_s.c"
	.text
	.p2align 2,,3
.globl zfunction_s
	.type	zfunction_s, @function
zfunction_s:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	andl	$-8372225, %eax
	sall	$4, %eax
	leave
	ret
	.size	zfunction_s, .-zfunction_s
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.6 20060404 (Red Hat 3.4.6-3)"
