section .data
msg1        db  'Foram lidos/escritos '
MSG1SIZE    EQU $-msg1
msg2        db  ' bytes.', 0Dh, 0Ah
MSG2SIZE    EQU $-msg2
nwln        db  0Dh, 0Ah
NWLNSIZE    EQU $-nwln

section .bss
n resb  12

section .text
global _start
_start:
    push eax
    push n
    push word 12
    call S_INPUT
    pop eax

    push eax
    push n
    push word 12
    call S_OUTPUT
    pop eax

    mov eax, 1
    mov ebx, 0
    int 80h


S_INPUT:
    push ebp    
    mov ebp, esp    
    sub edx, edx    

    mov eax, 3    
    mov ebx, 0    
    mov ecx, [ebp+10]    
    mov dx, [ebp+8]    
    int 80h    

    push eax
    push eax
    call MSGOUT
    pop eax

    pop ebp    
    ret 10

S_OUTPUT:
    push ebp    
    mov ebp, esp    
    sub edx, edx    

    mov eax, 4    
    mov ebx, 1    
    mov ecx, [ebp+10]    
    mov dx, [ebp+8]    
    int 80h    

    push eax
    push eax
    call MSGOUT
    pop eax

    pop ebp    
    ret 10


MSGOUT:
    push ebp
    mov ebp, esp
    push ebx
    push ecx
    push edx
    push esi

    mov eax, 4
    mov ebx, 1
    mov ecx, msg1
    mov edx, MSG1SIZE
    int 80h

    mov eax, [ebp+8]
    sub esp, 12
    mov ecx, esp
    push eax
    push ecx
    call CVTDCHX
    add esp, 12

    mov edx, eax
    mov eax, 4
    mov ebx, 1
    int 80h

    mov eax, 4
    mov ebx, 1
    mov ecx, msg2
    mov edx, MSG2SIZE
    int 80h

    pop esi
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret 4

CVTDCHX:
    push ebp
    mov ebp, esp
    push ebx
    push ecx
    push edx
    push esi

    mov eax, [ebp+12]   ; valor
    mov esi, [ebp+8]    ; ponteiro
    mov byte [esi], 0x0
    push esi
    cmp eax, 0
    jge CVTS 

    pop ebx
    mov byte [esi], 0x2d
    inc esi
    push esi
    mov byte [esi], 0x0
    neg eax

CVTS:
    cdq
    mov ebx, 10
    idiv ebx
    add dl, 0x30
    
CVTS_LOOP:
    mov cl, [esi]
    cmp cl, 0
    jz CVTS_FIM
    mov byte [esi], dl
    mov dl, cl
    inc esi
    jmp CVTS_LOOP

CVTS_FIM:
    mov byte [esi], dl
    inc esi
    mov byte [esi], 0x0
    pop esi
    push esi

    cmp eax, 0
    jz CVTDCHX_CNT
    jmp CVTS

CVTDCHX_CNT:
    pop esi
    mov eax, 0
    mov esi, [ebp+8]

CVTDCHX_CNT_LOOP:
    cmp byte [esi], 0x0
    jz CVTDCHX_FIM
    inc eax
    inc esi
    jmp CVTDCHX_CNT_LOOP

CVTDCHX_FIM:
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret 8