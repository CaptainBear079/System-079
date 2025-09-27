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
; This bootloader loads the bootmanager for the system
; Selection for which system to boot on will be done by the bootmanager
;
; Systems the bootmanager will load
; - System 079 / Chaos OS
; - Other systems like bootloader games
;
; Stage 1
; MBR (Master Boot Record)
; Bootmanager target location 0x7E00
;

[BITS 16]   ; 16-bit mode
[ORG 0x7C00] ; Set origin to 0x7C00

%define ENDL 0x0D, 0x0A

_start:
jmp short main
nop

main:
; Data segments and stack setup (interrupts disabled)
cli
mov ax, 0
mov ds, ax
mov es, ax

mov ss, ax
mov sp, 0x07C00 ; up to 0x05000
sti

; save boot drive from dl
mov [boot_drive], dl

; DAP setup
mov byte [dap+0], 0x10
mov byte [dap+1], 0x00
mov word [dap+2], 0x00016
mov word [dap+4], 0x7E00
mov word [dap+6], 0x0000

mov si, PARTITION_TABLE
mov cx, 4

find_aktiv_part_entry:
cmp byte[si], 0x80
je save_aktiv_part_entry
add si, 16
loop find_aktiv_part_entry
mov ah, 0x1
jmp ERROR

save_aktiv_part_entry:
; Read LBA
mov [aktiv_part_entry], si
mov ax, [si+8]
mov dx, [si+10]
mov word [dap+8], ax
mov word [dap+10], dx
xor ax, ax
xor dx, dx
mov word [dap+12], ax
mov word [dap+14], dx

; Read the sector with Stage 2
; Check for EDD support
clc
mov ah, 0x41
mov bx, 0x55AA
mov dl, [boot_drive]
int 0x13
jc no_edd
cmp bx, 0xAA55
jne no_edd

; Read Sector
mov ah, 0x42
mov dl, [boot_drive]
mov si, dap
int 0x13
jc read_sector_error

mov dl, [boot_drive] ; Stage 2 expects dl to contain boot drive

jmp 0x0000:0x7E00

read_sector_error:
mov ah, 2
jmp ERROR

no_edd:
mov ah, 3
jmp ERROR

print_string:
mov ah, 0x0E
jmp .next_char

.next_char:
lodsb
cmp al, 0
je .done
int 0x10
jmp .next_char
.done:
mov ah, 0
ret

ERROR:
cmp ah, 1
je .ERROR__1
cmp ah, 2
je .ERROR__2
cmp ah, 3
je .ERROR__3

.ERROR__1:
mov si, ERROR_1
call print_string
jmp InfinteLoop

.ERROR__2:
mov si, ERROR_2
call print_string
jmp InfinteLoop

.ERROR__3:
mov si, ERROR_3
call print_string
jmp InfinteLoop

InfinteLoop:
jmp $

; Boot text
ERROR_1: db 'No bootable partition entries. Please restart the PC. ERROR CODE 0x000001', ENDL, 0
ERROR_2: db 'LBA Reading error. Please restart the PC. ERROR CODE 0x000002', ENDL, 0
ERROR_3: db 'No EDD support. Please restart the PC. ERROR CODE 0x000003', ENDL, 0

; Boot data space
boot_drive: db 0
aktiv_part_entry: dw 0
; DAP
dap: times 16 db 0

times 446-($-$$) db 0 ; Fill to MBR offsets

PARTITION_TABLE:
; Offset to 0x7C00: 0x1BE
db 0x80                   ; Bootflag (0x80 = aktiv)
db 0x01, 0x01, 0x00       ; Start CHS (Dummywerte)
db 0x0B                   ; Partitions-Typ (Fat32)
db 0xFE, 0xFF, 0xFF       ; End CHS (Dummywerte)
dd 512                    ; Start LBA
dd 8                      ; Anzahl Sektoren

times 510-($-$$) db 0 ; Fill remaining partition table entries
; Boot signature
db 0x55
db 0xAA