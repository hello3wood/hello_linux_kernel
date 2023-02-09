        .text
        .align 4
        .type syscall_write,#function
        .globl syscall_write

        .text
        .align

#define __NR_WRITE	64

syscall_write:
	mov x8, #(__NR_WRITE)
	svc #0x0
	ret
