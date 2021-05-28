	.file	"jc-shell.c"
	.text
	.globl	mutex
	.bss
	.align 32
	.type	mutex, @object
	.size	mutex, 40
mutex:
	.zero	40
	.globl	Exit
	.align 4
	.type	Exit, @object
	.size	Exit, 4
Exit:
	.zero	4
	.globl	numChildren
	.align 4
	.type	numChildren, @object
	.size	numChildren, 4
numChildren:
	.zero	4
	.globl	list
	.align 8
	.type	list, @object
	.size	list, 8
list:
	.zero	8
	.section	.rodata
.LC0:
	.string	"clear"
	.align 8
.LC1:
	.string	"\033[31m\nThis program doesn't need any args\033[m\n"
.LC2:
	.string	"\t\t\033[0;35mJC-SHELL V.1.2\033[0m\n"
.LC3:
	.string	"Insert your commands:"
	.align 8
.LC4:
	.string	"\033[31mError creating monitor thread\033[m"
.LC5:
	.string	"\033[m"
.LC6:
	.string	"\033[31m"
	.align 8
.LC7:
	.string	"%sChild process creation failed.%s\n"
.LC8:
	.string	"\033[0;31mInexisting file\033[m"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$208, %rsp
	movl	%edi, -196(%rbp)
	movq	%rsi, -208(%rbp)
	leaq	.LC0(%rip), %rdi
	call	system@PLT
	cmpl	$1, -196(%rbp)
	jg	.L2
	movq	-208(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L3
.L2:
	leaq	.LC1(%rip), %rdi
	call	puts@PLT
.L3:
	movl	$0, numChildren(%rip)
	movl	$0, %eax
	call	process_new@PLT
	movq	%rax, list(%rip)
	leaq	.LC2(%rip), %rdi
	call	puts@PLT
	leaq	.LC3(%rip), %rdi
	call	puts@PLT
	leaq	-192(%rbp), %rax
	movl	$0, %ecx
	movq	monitor_Thread@GOTPCREL(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	cmpl	$-1, %eax
	jne	.L4
	leaq	.LC4(%rip), %rdi
	call	puts@PLT
	movl	$12, %edi
	call	exit@PLT
.L4:
	leaq	-112(%rbp), %rdx
	leaq	-176(%rbp), %rax
	movl	$100, %ecx
	movl	$7, %esi
	movq	%rax, %rdi
	call	readLineArguments@PLT
	movl	%eax, -4(%rbp)
	movq	stdin(%rip), %rax
	movq	%rax, %rdi
	call	feof@PLT
	testl	%eax, %eax
	je	.L5
	movq	-176(%rbp), %rax
	movl	$1953069157, (%rax)
	movb	$0, 4(%rax)
	jmp	.L6
.L5:
	cmpl	$0, -4(%rbp)
	jle	.L14
.L6:
	movq	-176(%rbp), %rax
	movq	%rax, %rdi
	call	command@PLT
	cmpl	$64, %eax
	je	.L8
	cmpl	$64, %eax
	jg	.L9
	testl	%eax, %eax
	je	.L10
	cmpl	$1, %eax
	je	.L11
	jmp	.L9
.L10:
	movl	$1, Exit(%rip)
	movq	-192(%rbp), %rax
	leaq	-184(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movq	list(%rip), %rax
	movq	%rax, %rdi
	call	process_print@PLT
	movq	list(%rip), %rax
	movq	%rax, %rdi
	call	process_destroy@PLT
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_destroy@PLT
	movq	stdin(%rip), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	movl	$0, %edi
	call	exit@PLT
.L8:
	leaq	.LC0(%rip), %rdi
	call	system@PLT
	leaq	.LC2(%rip), %rdi
	call	puts@PLT
	jmp	.L7
.L11:
	call	fork@PLT
	movl	%eax, -8(%rbp)
	cmpl	$-1, -8(%rbp)
	jne	.L12
	movq	stderr(%rip), %rax
	leaq	.LC5(%rip), %rcx
	leaq	.LC6(%rip), %rdx
	leaq	.LC7(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	jmp	.L7
.L12:
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_lock@PLT
	movl	numChildren(%rip), %eax
	addl	$1, %eax
	movl	%eax, numChildren(%rip)
	movl	$0, %edi
	call	time@PLT
	movq	%rax, %rdx
	movq	list(%rip), %rax
	movl	-8(%rbp), %ecx
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	insert_new_process@PLT
	leaq	mutex(%rip), %rdi
	call	pthread_mutex_unlock@PLT
	cmpl	$0, -8(%rbp)
	jne	.L15
	movq	-176(%rbp), %rax
	leaq	-176(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	execv@PLT
	jmp	.L15
.L9:
	leaq	.LC8(%rip), %rdi
	call	puts@PLT
	jmp	.L4
.L14:
	nop
	jmp	.L4
.L15:
	nop
.L7:
	jmp	.L4
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
