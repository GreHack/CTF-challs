BITS 64

OFFSET equ 24
; flag = GH19{The_broken_ELF}
segment .data

    chaine_bj db 39, "Bienvenue dans ce challenge de reverse",10, 0
    passwd db 20, 0xe2, 0x58, 0x47, 0x0d, 0x8f, 0x80, 0x2a, 0x79, 0x73, 0x76, 0x3f, 0xc0, 0x7d, 0x39, 0x82, 0x9b, 0x15, 0x60, 0x5a, 0x91, 0
    question db 26, "Please enter the password",10, 0
    try db 19, "Sorry, try again",10, 0
    well db 9, "Congrats",10,0

    tmp db 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

    check db 16, 0x43, 0x26, 0xc7, 0x31, 0x59, 0xe8, 0x9d, 0x09, 0x29, 0x7a, 0xe5, 0x24, 0xa6, 0x22, 0x1a, 0x3a, 0x29, 0x7a, 0xe5, 0x24, 0xa6, 0x22, 0x1a, 0x3a

segment .text

global _start

_start:
    mov ecx, chaine_bj
    call puts
    mov ecx, question
    call puts
    call get_str
    mov eax, tmp+1
    call dechiff
    xor rdi, rdi

checksum:  ;loop to compress 5 times;  
    mov rax, tmp+1
    call last_crypt
    cmp edi, 5
    jl checksum

    mov eax, tmp+1
    mov ebx, check+1
    mov ecx, 8
    call str_cmp_init
    mov ecx, well
    call puts
    mov eax, 1
    int 0x80


exit:
    mov ecx, try
    call puts
    mov eax, 1
    int 0x80

puts:           ;print a string, 1 param : ecx = string;
    ;pusha
    mov eax, 4
    mov ebx, 0
    xor edx, edx
    mov dl, [ecx]
    add ecx, 1
    int 0x80
    ;popa
    ret

  
get_str:
    mov eax, 3
    mov ebx, 1
    mov ecx, tmp+1
    mov edx, 32
    int 0x80
    ret

str_cmp_init:   ;eax = str1, ebx = str2, ecx=size;
    xor esi, esi
    xor edx, edx

str_cmp:
    mov dl, [eax]
    cmp dl, [ebx]
    jne exit
    inc eax
    inc ebx
    inc esi
    cmp esi, ecx
    jne str_cmp
    ret

dechiff:
    cmp byte [eax], 0
    je dechiff_2_init
    xor byte [eax], 0x38
    add eax, 1
    jmp dechiff

dechiff_2_init:
    mov edi, 0x400000
    add edi, OFFSET
    mov esi, edi
    add esi, 3
    mov eax, tmp+1

dechiff_2:
    cmp byte [eax], 0
    je dechiff_3_init
    mov dl, [edi]
    sub [eax], dl
    inc rax
    inc rdi
    cmp edi, esi 
    jne dechiff_2
    sub edi, 3
    jmp dechiff_2

dechiff_3_init:
    mov edi, 0x400000
    mov bl, [edi] 
    mov eax, tmp+1

dechiff_3:
    cmp byte [eax], 0
    je dechiff_4_init
    xor [eax], bl
    inc rax
    jmp dechiff_3

dechiff_4_init:
    inc edi
    mov esi, edi
    add esi, 3
    mov eax, tmp+1

dechiff_4:
    
    cmp byte [eax], 0
    je return
    mov dl, [edi]
    add [eax], dl
    inc rax
    inc rdi
    cmp edi, esi
    jne dechiff_4
    sub edi, 3
    jmp dechiff_4

last_crypt:  ; Compress the ciphered input
    mov bl, [rax+1]
    add [rax], bl
    inc eax
    cmp byte [rax+1], 0
    jne last_crypt

    inc rdi
    mov byte [rax], 0

return:
    ret
