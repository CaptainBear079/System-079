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
    call main
    xor rdi, rdi
    mov rdi, rax
    mov rax, 60
    syscall

main:                              ; int main()
    push rbp
    mov rbp, rsp

    ; With variable metadata (for debugging)
    mov [rbp-1], 00100110b ; 2 bit unused, 1 bit signed, 5 bit type
    mov [rbp-5], 0

    mov rax, 0                     ; return 0;

    mov rsp, rbp
    pop rbp
    ret

section .bss
bss_start:
bss_end:
section .rodata