;
; System 079 BootManagerloader v1.0.0
; Date: 08 August 2025 Written by Captain Bear
; Email: louis.ritz12@gmail.com
; GitHub: Currently private
;
; By using, copying, or/and modifying this code, or the assembled machine code, you agree to the following:
; - You give credit to the original author(s)
; - You document all your changes
; - You don't remove or/and modify this notice
; - You add your own notice for your modifications and credit the changes author(s)
; - You agree to the extern license if there is one
; - You agree that the original author(s) do not hold any responsibility for your modifications
; - You are aware that this code is provided as-is without any warranty
; - You do not make money with it, when you want to make money with it, you have to ask for permission (email)
;      Note: Will be granted in most cases (as example: game development, most software, most cases of commercial use in operating systems), but you have to ask!
; - You do not make fun of this code or it's author(s), this also counts for the author(s) of changes and/or the users and community,
;      this includes any form of harassment or discrimination
; - You do not use this code or the assembled machine code for any illegal purposes
;
; Systems the bootmanager will load
; - System 079 / Chaos OS
; - Other systems like bootloader games
;

extern __bss_start
extern __end

extern gdt_start
extern gdt_end
extern gdt_descriptor

extern call_c

section .text
; Switch to protected mode
global switch_to_protected_mode
switch_to_protected_mode:
    [bits 16]
    cli
    lgdt [gdt_descriptor] ; Load GDT
    call EnableA20
    mov eax, cr0
    or  eax, 1
    mov cr0, eax

    jmp dword 0x08:protected_mode_entry

protected_mode_entry:
    [bits 32]
    mov eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0xFFF0
    mov ebp, esp
 
    sti

    mov edi, __bss_start
    mov ecx, __end
    sub ecx, edi
    mov al, 0
    cld
    rep stosb
    cld

    jmp dword 0x08:call_c

EnableA20:
    [bits 16]
    push ax
    in al, 0x92
    or al, 00000010b
    out 0x92, al
    pop ax
    ret

global asm_input
global asm_output
asm_input:
    [bits 32]
    ; Input for C
    ; Stack frame and callee-saved registers
    push ebp
    mov ebp, esp
    push edx
    

    ; Get port (uint16_t) in dx and value (uint8_t) in al
    mov dx, [ebp + 8]
    xor eax, eax
    in al, dx

    ; Restore stack frame
    pop edx
    pop ebp
    ret

asm_output:
    [bits 32]
    ; Output for C
    ; Stack frame and callee-saved registers
    push ebp
    mov ebp, esp
    push edx
    push eax

    ; Get port (uint16_t) in dx and value (uint8_t) in al
    mov dx, [ebp + 8]
    mov al, [ebp + 12]
    out dx, al

    ; Restore stack frame
    pop eax
    pop edx
    pop ebp
    ret

global asm_jump_to_kernel
asm_jump_to_kernel:
    mov eax, [esp+4]
    jmp eax