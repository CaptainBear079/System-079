section .data
section .text
global _start
_start:
    ; Zero out bss
    mov rdi, bss_start
    mov rcx, bss_end
    sub rcx, rdi
    xor rax, rax
    rep stosb

    ; Set global variables
    mov byte [__GLOBALVAR_var], 00100110b ; Explained in main example
    ; mov dword [__GLOBALVAR_var+1], 0            ; Zero set not needed just save it in bss
                                          ; actual var = __GLOBALVAR_name + meta offset if activated meta

    ; Execute code
    call main
    xor rdi, rdi
    mov rdi, rax
    mov rax, 60
    syscall

main:                                     ; int main()
    push rbp
    mov rbp, rsp

    ; With variable metadata (for debugging)
    mov [rbp-1], 00100110b                ; 2 bit unused, 1 bit signed, 5 bit type
    mov [rbp-5], 0

    mov rax, 0                            ; return 0;

    mov rsp, rbp
    pop rbp
    ret

section .bss
bss_start:
__GLOBALVAR_var: db 0
                 dd 0
bss_end:
section .rodata