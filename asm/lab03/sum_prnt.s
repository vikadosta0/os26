section .data
    n       equ 12
    A       dd 1,2,3,4,5,6,7,8,9,10,11,12
    B       dd 12,11,10,9,8,7,6,5,4,3,2,1
    i       dd 0
    j       dd 0

section .bss
    C       resd 12

section .text
    global  _start
    extern  E_prnt

_start:
    push    dword A
    mov     eax, n
    push    eax
    call    E_prnt
    add     esp, 8

    push    dword B
    mov     eax, n
    push    eax
    call    E_prnt
    add     esp, 8

    mov     ecx, n
    xor     ebx, ebx
sum_loop:
    mov     eax, [A + ebx*4]
    add     eax, [B + ebx*4]
    mov     [C + ebx*4], eax
    inc     ebx
    loop    sum_loop

    push    dword C
    mov     eax, n
    push    eax
    call    E_prnt
    add     esp, 8

L_exit:
    mov     eax, 1
    xor     ebx, ebx
    int     0x80
