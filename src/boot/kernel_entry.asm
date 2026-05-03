[bits 32]

; --- Multiboot 1 Header (Без него Limine не запустит ядро!) ---
section .multiboot
align 4
    dd 0x1BADB002             ; Magic number
    dd 0x03                   ; Flags (align + meminfo)
    dd -(0x1BADB002 + 0x03)   ; Checksum

section .text
global _start
extern kmain

_start:
    mov esp, 0x90000          ; Стек
    call kmain
    jmp $
