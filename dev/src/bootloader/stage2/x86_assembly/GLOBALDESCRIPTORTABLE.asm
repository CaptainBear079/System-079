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

; Global Descriptor Table
global gdt_start
global gdt_end
global gdt_descriptor
section .data
[bits 16]

gdt_start:
    dq 0 ; Null descriptor

    ; 32 Bit Code segment
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

    ; 32 Bit Data segment
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

    ; 16 Bit Code segment
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 00000000b
    db 0x00

    ; 16 Bit Data segment
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 00000000b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1   ; size of GDT (limit)
    dd gdt_start                 ; address of GDT