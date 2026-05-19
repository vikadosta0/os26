section .data
const1 equ 5
var1 dd 7

section .text
global _start

_start:
mov eax, const1
mov ebx, [var1]
sub eax, ebx
mov ebx, eax

L_exit:
mov eax, 1
int 80h
