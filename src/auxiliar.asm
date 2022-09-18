mult:
    mov ebx, [d?]   ; passa o valor da variavel para ebx
    cdq             ; extende o sinal para edx
    imul ebx        ; realiza a multiplicação com sinal
    jnc l?          ; pula se não houve carry
    mov eax, 1      ; processo de encerrar programa
    mov ebx, 0
    int 80h
l?: ; adicionar linha seguinte à lista de labels

divi:
    mov ebx, [d?]
    cdq
    idiv ebx

; pesquisar sobre variáveis locais

s_input:
    push ebp
    mov ebp, esp
    sub edx, edx
    mov eax, 3
    mov ebx, 0
    mov ecx, [ebp+10]
    mov dx, [ebp+8]
    int 80h
    pop ebp
    ret 10
