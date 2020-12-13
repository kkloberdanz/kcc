	.file	"hello.c"
	.text
.LC0:
	.string	"hello world!"

	.globl	main
	.globl	_start
	.globl	_end
	.type	main, @function

_start:
main:
.LFB0:
	.cfi_startproc
	pushq	%rax
	pushq	%rdi
	movl	$.LC0, %edi
	call	puts
	popq	%rdi
	popq	%rax
	xorl	%eax, %eax
	.cfi_endproc
_end:
	movl	%eax, %ebx
	movl	$0x1, %eax
	int		$0x80
