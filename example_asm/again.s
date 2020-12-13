	.file	"again.c"
	.text
.LC0:
	.string	"hello again!"

	.globl	again
	.type	again, @function

again:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	movq	%rsp, %rbp

	pushq	%rdi
	movq	$.LC0, %rdi
	call	puts
	popq	%rdi
	xorq	%rax, %rax
	leave
	ret
	.cfi_endproc
