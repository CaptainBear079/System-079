;
; ChaosLang target assembly examples
;     by Hacklord Captain Bear (CaptainBear079 on GitHub)
;     Email: hacklordcaptainbear@gmail.com
;
; ------ Warning and License/Terms of Use --------
; !!! THIS CODE IS PROVIDED AS IT IS, DO NOT TRY TO SUE US !!!
; !!! THIS CODE IS JUST A EXAMPLE, DO NOT ASSEMBLE AND/OR RUN IT !!!
; This example shows how ChaosLang looks when translated to assembly.
; By comtinuing to read and/or use this code you agree to the following:
; 1. You understand that this code is for educational purposes only.
; 2. You take full responsibility for any damage caused by the assembled and/or run code.
; 3. You will not spread this code without this notice.
; 4. You will not spread a modified version of this code without clearly marking it as modifies.
; 5. You will not use this code for content farming, like calling this code bad or spreading misinformation.
;
section .data
section .text
extern pthread_create
extern pthread_join
global _start
_start:
    ; Zero out bss
    mov rdi, bss_start
    mov rcx, bss_end
    sub rcx, rdi
    xor rax, rax
    rep stosb

    ; Execute code
    call main
    xor rdi, rdi
    mov rdi, rax
    mov rax, 60
    syscall

main:                                     ; int main()
    push rbp
    mov rbp, rsp

    ; pthread_create
    lea rdi, [label_001__pthread_t]
    xor rsi, rsi
    lea rdx, [rel thread_function]
    lea rcx, [thread_function_args]

    xor rax, rax                          ; return 0;

    mov rsp, rbp
    pop rbp
    ret

thread_function:
    push rbp
    mov rbp, rsp

    xor rax, rax

    mov rsp, rbp
    pop rbp
    ret

section .bss
bss_start:
label_001__pthread_t: resq 1              ; Space for thread handle
bss_end:
section .rodata