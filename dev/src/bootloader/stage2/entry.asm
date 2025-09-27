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
; Stage 2
; Switch to protected mode
; Jump to C code
;

bits 16
; Bootmanager asm c setup using GCC

section .entry

extern __bss_start
extern __end

extern _cstart
extern switch_to_protected_mode
global bootmanager_entry
global call_c

bootmanager_entry:
    [bits 16]
    mov [bootdrive], dl

    ; Setup
    cli
    mov ax, ds ; DS should be zero since stage 1 just set it up but didn't change it
    mov ss, ax
    mov sp, 0xFFF0
    mov bp, sp
    sti

    call switch_to_protected_mode

call_c:
    [bits 32]

    ; Pass parameters to C code _cstart(uint32_t boot_drive)
    mov dl, [bootdrive]
    xor dh, dh
    push edx
    call _cstart ; Call C code to handle the boot manager logic

    cli
    hlt

section .data
; System Information
global bootdrive
bootdrive: db 0