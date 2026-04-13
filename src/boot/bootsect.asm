; ------------------------------------------------------------------------------
; Guide:    01-KERNEL
; File:     boot/bootsect.asm
; Title:    Обновленный загрузчик с поддержкой расширенного ядра
; ------------------------------------------------------------------------------

[org 0x7c00]

KERNEL_OFFSET equ 0x1000    ; Смещение в памяти для ядра

    mov [BOOT_DRIVE], dl    ; Сохраняем номер загрузочного диска от BIOS
    mov bp, 0x9000          ; Устанавливаем стек подальше от кода
    mov sp, bp

    mov bx, MSG_REAL_MODE
    call print_string

    call load_kernel        ; Загружаем ядро и данные с диска
    call switch_to_pm       ; Прыгаем в 32-битный режим
    jmp $

; Инклуды. Если используешь Makefile с флагом -i, пути останутся такими.
%include "print_string.asm"
%include "print_hex.asm"
%include "disk_load.asm"
%include "print_string_pm.asm"
%include "switch.asm"
%include "gdt.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print_string

    mov bx, KERNEL_OFFSET   ; Куда грузим
    ; Увеличиваем количество секторов. 
    ; 16 было мало, поставим 50, чтобы влезло ядро + таблица ФС + первые файлы.
    mov dh, 50              
    mov dl, [BOOT_DRIVE]
    call disk_load          
    ret

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm    
    
    ; Прыгаем в ядро. 
    ; Используем call, чтобы если ядро сделает ret, мы вернулись в jmp $
    call KERNEL_OFFSET      
    jmp $

; Данные
BOOT_DRIVE:      db 0
MSG_REAL_MODE:   db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE:   db "Landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL: db "Loading kernel & FS data...", 0

; Заполнение до 512 байт
times 510-($-$$) db 0
dw 0xaa55