[bits 32]
global _start
extern kmain

_start:
    ; Важно: выравниваем стек, если это требуется для C (иногда помогает)
    mov esp, 0x90000 
    
    call kmain       ; Вызов C функции
    jmp $            ; Зависаем, если kmain вернет управление