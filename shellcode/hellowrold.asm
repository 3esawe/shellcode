section .data         ;Data segment 

msg db  "Hello wrold", 0xa0 ; the string and newlline 

section .text ; Text segment 

global _start ; Deafault entry point for ELF 

_start:
; Syscall: write(1, msg, 14)
mov eax , 4
mov ebx, 1
mov ecx, msg
mov edx , 14
int 0x80

; Syscall : exit(0)
mov eax, 1
mov ebx, 0
int 0x80

