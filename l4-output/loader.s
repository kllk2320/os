global loader

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER

KERNEL_STACK_SIZE equ 4096




section .text:
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

	extern	kmain 
loader:
    mov eax, 0xCAFEBABE
	mov esp, kernel_stack + KERNEL_STACK_SIZE 	; point esp to the start of the stack (end of memory area)
	call kmain 					;  Call the main function 
.loop:
    jmp .loop


section .bss:
align 4
kernel_stack:
	resb KERNEL_STACK_SIZE
