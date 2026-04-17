     1                                  section .data
     2                                  const1 equ 5
     3 00000000 07000000                var1 dd 7
     4                                  section .text
     5                                  global _start
     6                                  _start:
     7 00000000 B805000000              mov eax, const1
     8 00000005 8B1D[00000000]          mov ebx, [var1]
     9 0000000B 29D8                    sub eax, ebx
    10                                  L_exit:
    11 0000000D B801000000              mov eax, 1
    12 00000012 31DB                    xor ebx, ebx
    13 00000014 CD80                    int 80h
