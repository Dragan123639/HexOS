; boot.s - multiboot header + jump to kernel_main

section .multiboot
align 4
	dd 0x1BADB002
	dd 0x00
	dd -(0x1BADB002 + 0x00)

section .text
[bits 32]
[extern kernel_main]
global _start

_start:
    ; call your kernel entry point
    call kernel_main
    cli
.hang:
    hlt
    jmp .hang

