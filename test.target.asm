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