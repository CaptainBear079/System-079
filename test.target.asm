section .data
section .text
global _start
extern main
_start:
    call main
    mov rax, 60
    xor rdi, rdi
    syscall

main:                              ; int main()
    push rbp
    mov rbp, rsp

    ; With variable listing
    ; mov dword [current_varlist_entry], int_main_x_0000
    ; mov [current_varlist_entry+4], "x"
    mov dword [int_main_x_0000], 0 ; x = int_main_x_0000 int x = 0;

    mov rax, 0                     ; return 0;

    mov rsp, rbp
    pop rbp
    ret

section .bss
section .rodata