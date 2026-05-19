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
    11 0000000B 01D8                    add eax, ebx
    12                                  
    13                                  L_exit:
    14 0000000D B801000000              mov eax, 1
    15 00000012 31DB                    xor ebx, ebx
    16 00000014 CD80                    int 80h
