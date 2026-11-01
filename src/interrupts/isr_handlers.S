.intel_syntax noprefix

.macro NOER_ISR vector
.global isr_\vector
isr_\vector:
	push 0
	push \vector
	jmp isr_common
.endm

.macro ER_ISR vector
.global isr_\vector
isr_\vector:
	push \vector
	jmp isr_common
.endm

NOER_ISR 0
NOER_ISR 1
NOER_ISR 2
NOER_ISR 3
NOER_ISR 4
NOER_ISR 5
NOER_ISR 6
NOER_ISR 7
ER_ISR   8
NOER_ISR 9
ER_ISR   10
ER_ISR   11
ER_ISR   12
ER_ISR   13
ER_ISR   14
ER_ISR   15
ER_ISR   16
ER_ISR   17

NOER_ISR 32
NOER_ISR 33




isr_common:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	
	mov rdi, rsp
	call isr_exception_handler

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax

	add rsp, 16
	iretq

