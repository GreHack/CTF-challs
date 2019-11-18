.section        .chall,"wax",@progbits
.globl  chall
.type   chall, @function
chall:
	movl    %edx, %r8d
	xorl    %ecx, %ecx
.L2:
	movl    %ecx, %eax
	xorl    %edx, %edx
	divl    %r8d
	movb    (%rsi,%rdx), %al
	xorb    %al, (%rdi,%rcx)
	incq    %rcx
	jmp     .L2
	nop
.size   chall, .-chall
