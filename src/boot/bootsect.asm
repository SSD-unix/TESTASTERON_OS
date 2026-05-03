; boot/bootsect.asm
[org 0x7c00]
jmp short start
nop

; =====================================================================
; === ФЕЙКОВЫЙ BPB (BIOS Parameter Block) ===
; Магическая структура, которая заставляет строгие BIOS (HP/Dell)
; признать эту флешку легитимным загрузочным диском.
; =====================================================================
oem_name            db "TESTOS  "  ; 8 байт - Имя OEM
bytes_per_sector    dw 512
sectors_per_cluster db 1
reserved_sectors    dw 1
num_fats            db 2
root_dir_entries    dw 224
total_sectors_16    dw 2880
media_descriptor    db 0xF0
sectors_per_fat     dw 9
sectors_per_track   dw 18
num_heads           dw 2
hidden_sectors      dd 0
total_sectors_32    dd 0
drive_number        db 0x80        ; 0x80 = Жесткий диск или USB-флешка
reserved            db 0
ext_boot_signature  db 0x29
volume_serial       dd 0x12345678
volume_label        db "TESTOSTERON" ; 11 байт - Имя тома
fs_type             db "FAT12   "    ; 8 байт - Тип ФС
; =====================================================================


start:
    ; 1. Настройка сегментов памяти
    cli
    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; 2. Сохраняем номер диска (BIOS кладет его в регистр DL при старте)
    ; Это критически важно для реального железа!
    mov [BOOT_DRIVE], dl

    ; 3. Читаем ядро с флешки в оперативную память
    mov ah, 0x02        ; Команда BIOS: чтение секторов
    mov al, 45          ; Читаем 45 секторов (~23 КБ памяти). Если ядро вырастет, увеличим.
    mov ch, 0           ; Цилиндр 0
    mov dh, 0           ; Головка 0
    mov cl, 2           ; Начинаем со 2-го сектора (1-й — это сам загрузчик)
    mov dl, [BOOT_DRIVE]; Читаем именно с той флешки, с которой загрузились
    mov bx, 0x1000      ; Адрес в оперативной памяти (0x1000), куда ляжет ядро
    int 0x13

    ; 4. Инициализация GDT и переход в 32-битный режим (Protected Mode)
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp 0x08:init_pm

[bits 32]
init_pm:
    ; 5. Настройка 32-битных сегментов
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp

    ; 6. Прыжок в функцию kmain (через твой kernel_entry)
    call 0x1000
    jmp $               ; Бесконечный цикл на случай сбоя

; Переменная для хранения диска
BOOT_DRIVE db 0

; =====================================================================
; === Таблица GDT (Global Descriptor Table) ===
; =====================================================================
gdt_start:
gdt_null:
    dd 0, 0
gdt_code:
    dw 0xffff, 0
    db 0, 10011010b, 11001111b, 0
gdt_data:
    dw 0xffff, 0
    db 0, 10010010b, 11001111b, 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; =====================================================================
; Заполняем нулями остаток 512-байтного сектора и ставим магическую подпись
; =====================================================================
times 510-($-$$) db 0
dw 0xaa55
