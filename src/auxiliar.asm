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
input:
    ; recebe ponteiro de onde ficará o int
    ; cria variavel global/local para receber número em ascii
    ; receber número
    ; checar se é o fim
    ; carregar o primeiro número
    ; subtrair 0x30
    ; somar no acumulador
    ; voltar
    ; se for o fim, salvar número no ponteiro
    ; voltar

    push dop1
    push word op2
    call sum
s_input:
    push ebp
    mov ebp, esp
    push eax
    sub edx, edx
    mov eax, 3
    mov ebx, 0
    mov ecx, [ebp+10]
    mov dx, [ebp+8]
    int 80h
    pop eax
    pop ebp
    ret 10
