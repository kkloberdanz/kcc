	.file	"hello.c"
	.text
.LC0:
	.string	"hello world!"

	.globl	main
	.globl	inc
	.globl	_start
	.globl	_end
	.type	main, @function

main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	movq	%rsp, %rbp

	pushq	%rdi
	movq	$.LC0, %rdi
	call	puts
	popq	%rdi
	xorq	%rax, %rax
	call	again
	leave
	ret
	.cfi_endproc
_start:
	call main
_end:
	movq	%rax, %rbx
	movq	$0x1, %rax
	int		$0x80
