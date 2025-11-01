[bits 32]

;
; extern void __attribute__((cdecl)) i686_Load_GDT(GDT_DESCRIPTOR GDT_Descriptor, uint16_t CodeSegment, uint16_t DataSegment);
;
global i686_Load_GDT
i686_Load_GDT:
    ; Stack frame
    push ebp
    mov ebp, esp

    ; Load GDT
    mov eax, [ebp+8]
    lgdt [eax]

    ; Load code segment
    mov eax, [ebp+12]
    push eax
    push .reload_cs
    retf

.reload_cs:
    ; Load data segment
    mov ax, [ebp+16]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Delete stack frame
    mov esp, ebp
    pop ebp
    ret