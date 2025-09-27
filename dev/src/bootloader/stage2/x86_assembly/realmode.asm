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

section .text
extern __SYS_SCREEN_X
extern __SYS_SCREEN_Y

%macro EnterRealMode 0
[bits 32]
cli
jmp word 0x18:.rmode16

.rmode16:
[bits 16]
mov eax, cr0
and al, ~1
mov cr0, eax

jmp word 0x0:.rmode

.rmode:
mov ax, 0
mov ds, ax
mov ss, ax

sti

%endmacro

%macro EnterProtectedMode 0
[bits 16]
cli
mov eax, cr0
or  eax, 1
mov cr0, eax

jmp dword 0x08:.pmode_entry

.pmode_entry:
[bits 32]
mov eax, 0x10
mov ds, ax
mov ss, ax

sti

%endmacro

; Convert linear address to segment:offset address
; Args:
;    1 - linear address
;    2 - (out) target segment (e.g. es)
;    3 - target 32-bit register to use (e.g. eax)
;    4 - target lower 16-bit half of #3 (e.g. ax)
%macro LinearToSegmentOffset 4
    mov %3, %1      ; linear address to eax
    shr %3, 4
    mov %2, %4
    mov %3, %1      ; linear address to eax
    and %3, 0xf
%endmacro

;
; extern void __attribute__((cdecl)) asm_m16_DisplayText(char* text);
;

global asm_m16_DisplayText
asm_m16_DisplayText:
[bits 32]
; Stack frame
push ebp
mov ebp, esp
push edx
push ecx

; Switch to real mode
EnterRealMode
[bits 16]

; Save regs
push bx
push si
push ax
push di
push dx

; Display Text
mov si, [bp+8] ; Pointer to string
mov cx, [bp+16] ; current column
mov dx, 0x3D4
; Set seg:offset to video memory
mov ax, 0xB800
mov es, ax
mov di, [bp+12]

; Print string
mov ah, 0x07   ; Text attribute (light grey on black)
jmp next_char

next_char:
lodsb
cmp al, 0
je .done
clc
cmp al, 0x0A
je .newline
stosw
inc cx
jmp next_char

jmp $

.newline:
mov al, ' '
clc
cmp cx, 80
je .newline_finished
stosw
inc cx
jmp .newline

.newline_finished:
mov cx, 0
inc di
inc di
inc dx
jmp next_char

.done:
mov bx, dx

; Restore regs
pop dx
pop di
pop ax
pop si

mov dx, bx

pop bx

; Switch to protected mode
EnterProtectedMode
[bits 32]

; Set return value
xor eax, eax        ; Clear EAX
mov ax, dx          ; Lower 16 bits
mov bx, cx
shl ebx, 16         ; Shift upper 16 bits
or eax, ebx         ; Combine both

; Delete stack frame
pop ecx
pop edx
mov esp, ebp
pop ebp

ret

;
; extern void __attribute__((cdecl)) asm_m16_int0x13_Reset(uint8_t drive);
;
global asm_m16_int0x13_Reset
asm_m16_int0x13_Reset:
[bits 32]
; Stack frame
push ebp
mov ebp, esp

; Switch to real mode
EnterRealMode
[bits 16]

; Save regs
push dx

; Int 0x13 Disk Reset
clc
mov dl, [bp+8]
mov ah, 0x00
int 0x13

; Restore regs
pop dx

; Switch to protected mode
EnterProtectedMode
[bits 32]

; Delete stack frame
mov esp, ebp
pop ebp

ret

;
; extern void __attribute__((cdecl)) asm_m16_int0x13_Get_Param(uint8_t drive);
;
global asm_m16_int0x13_Get_Param
asm_m16_int0x13_Get_Param:
[bits 32]
; Stack frame
push ebp
mov ebp, esp

; Switch to real mode
EnterRealMode
[bits 16]

; Save regs
push dx
push es
push di

; Int 0x13 Disk Get Parameters
.retry:
mov ax, 0
mov es, ax
mov di, ax
mov dl, [bp+8]
mov ah, 0x08
int 0x13
je .error
jmp .done

.error:
call asm_m16_int0x13_Reset
jmp short .retry

.done:
mov [int13_GetParam_CX], cx
mov [int13_GetParam_DX], dx

; Restore regs
pop di
pop es
pop dx

; Switch to protected mode
EnterProtectedMode
[bits 32]

; Delete stack frame
mov esp, ebp
pop ebp

ret

;
; extern void __attribute__((cdecl)) asm_m16_int0x13_Read(uint8_t drive, uint16_t cylinder, uint8_t head, uint8_t sector, uint8_t num_sectors, void* buffer);
;
global asm_m16_int0x13_Read
asm_m16_int0x13_Read:
[bits 32]
; Stack frame
push ebp
mov ebp, esp

; Switch to real mode
EnterRealMode
[bits 16]

; Save regs
push cx
push dx
push ebx

; Int 0x13 Disk Read
.retry:
mov al, [bp+16] ; num_sectors
mov ch, [bp+12] ; cylinder
mov cl, [bp+14] ; sector
mov dh, [bp+13] ; head
mov dl, [bp+8]  ; drive

LinearToSegmentOffset [bp+20], es, ebx, bx ; buffer to es:bx

mov ah, 0x02
int 0x13
je .error
jmp .done

.error:
call asm_m16_int0x13_Reset
jmp short .retry

.done:

; Restore regs
pop ebx
pop dx
pop cx

; Switch to protected mode
EnterProtectedMode
[bits 32]

; Delete stack frame
mov esp, ebp
pop ebp

ret

;
; extern void __attribute__((cdecl)) asm_m16_int0x13_EDD_Read(uint8_t drive, uint16_t upper_upper_lba, uint16_t upper_lower_lba, uint16_t lower_upper_lba, uint16_t lower_lower_lba, uint16_t num_sectors, void* buffer);
;                                                             [bp+8],        [bp+12],                  [bp+16],                  [bp+20],                  [bp+24],                  [bp+28],              [bp+32]
;
global asm_m16_int0x13_EDD_Read
asm_m16_int0x13_EDD_Read:
[bits 32]
; Stack frame
push ebp
mov ebp, esp
push eax
push ecx

; Switch to real mode
EnterRealMode
[bits 16]

; Int 0x13 with EDD
mov byte [dap+0],  0x10                    ; Size of Packet (0x10)
mov byte [dap+1],  0x00                    ; Reserved
mov ax, [bp+28]
mov word [dap+2],  ax                      ; Number of sectors to read
LinearToSegmentOffset [bp+32], es, eax, ax ; buffer to es:ax
mov word [dap+4],  ax                      ; Pointer to buffer
mov word [dap+6],  es                      ; Pointer to buffer (segment)
mov ax, [bp+24]
mov word [dap+8],  ax                      ; Starting LBA (lower low word)
mov ax, [bp+20]
mov word [dap+10], ax                      ; Starting LBA (lower high word)
mov ax, [bp+16]
mov word [dap+12], ax                      ; Starting LBA (upper low word)
mov ax, [bp+12]
mov word [dap+14], ax                      ; Starting LBA (upper high word)

mov ah, 0x42
mov dl, [bp+8]
mov si, dap
int 0x13

; Switch to protected mode
EnterProtectedMode
[bits 32]

; Delete stack frame
pop ecx
pop eax
mov esp, ebp
pop ebp

ret

;
; extern void __attribute__((cdecl)) asm_m16_int0x13_EDD_Get_Param(uint8_t drive, void* buffer);
;
global asm_m16_int0x13_EDD_Get_Param
asm_m16_int0x13_EDD_Get_Param:
[bits 32]
; Stack frame
push ebp
mov ebp, esp
push eax
push ecx

; Switch to real mode
EnterRealMode
[bits 16]

; Int 0x13 with EDD - Get Parameters
mov byte [dap+0],  0x10    ; Size of Packet (0x10)
mov byte [dap+1],  0x00    ; Reserved
mov word [dap+2],  0x00016 ; Number of sectors to read
mov word [dap+4],  0x7E00  ; Pointer to buffer
mov word [dap+6],  0x0000  ; Pointer to buffer (segment)
mov ax, [bp+20]
mov word [dap+8],  ax      ; Starting LBA (lower low word) from uint16_t lower_low_lba
mov ax, [bp+22]
mov word [dap+10], ax      ; Starting LBA (lower high word) from uint16_t lower_high_lba
mov ax, [bp+24]
mov word [dap+12], ax      ; Starting LBA (upper low word) from uint16_t upper_low_lba
mov ax, [bp+26]
mov word [dap+14], ax      ; Starting LBA (upper high word) from uint16_t upper_high_lba

mov ah, 0x42
mov dl, [bootdrive]
mov si, dap
int 0x13

; Switch to protected mode
EnterProtectedMode
[bits 32]

; Delete stack frame
pop ecx
pop eax
mov esp, ebp
pop ebp

ret

section .data
extern bootdrive
; Texts
RealModeEntry: db 'Entering Real Mode...', 0
; Int 0x13 Get Parameters
int13_GetParam_CX: dw 0
int13_GetParam_DX: dw 0
; Int 0x13 EDD
dap: times 16 db 0