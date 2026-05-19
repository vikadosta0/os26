     1                                  section .data
     2                                  const1 equ 5
     3 00000000 07000000                var1 dd 7
     4                                  
     5                                  section .text
     6                                  global _start
     7                                  
     8                                  _start:
     9 00000000 B805000000              mov eax, const1
    10 00000005 8B1D[00000000]          mov ebx, [var1]
    11 0000000B 29D8                    sub eax, ebx
    12 0000000D 89C3                    mov ebx, eax
    13                                  
    14                                  L_exit:
    15 0000000F B801000000              mov eax, 1
    16 00000014 CD80                    int 80h
