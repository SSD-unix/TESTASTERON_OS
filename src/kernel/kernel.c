/* kernel/kernel.c */
#include "../common.h"
#include "../drivers/screen.h"
#include "../drivers/lowlevel_io.h"
#include "./utils.h"

// Прототипы функций
void list_files();
void touch_file(char* filename);
void format_disk();
void get_user_input(char* buffer);

// --- Таблицы символов (Обычные и с Shift) ---
char scancode_to_char[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

char scancode_to_char_shift[] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '
};

// --- Вспомогательные функции ---

void kprint_int(int n) {
    if (n == 0) { kprint("0"); return; }
    char s[12];
    int i = 0;
    while (n > 0) {
        s[i++] = (n % 10) + '0';
        n /= 10;
    }
    s[i] = '\0';
    for (int j = 0; j < i / 2; j++) {
        char temp = s[j];
        s[j] = s[i - j - 1];
        s[i - j - 1] = temp;
    }
    kprint(s);
}

int get_ram_size_mb() {
    unsigned char low, high;
    port_byte_out(0x70, 0x30);
    low = port_byte_in(0x71);
    port_byte_out(0x70, 0x31);
    high = port_byte_in(0x71);
    int total_kb = ((high << 8) | low) + 1024;
    return total_kb / 1024;
}

// --- Обработка команд ---

void execute_command(char *input) {
    if (compare_string(input, "EXIT") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        __asm__ volatile("hlt");
    } 
    else if (compare_string(input, "CLEAR") == 0) {
        clear_screen();
    } 
    else if (compare_string(input, "HELLO") == 0) {
        kprint("Hello user! Welcome to the shell.\n");
    } 
    else if (compare_string(input, "HELP") == 0) {
        kprint("Commands: HELLO, CLEAR, EXIT, LS, FORMAT, TOUCH, INFO\n");
    } 
    else if (compare_string(input, "INFO") == 0) {
        kprint("TESTASTERON 1.0 alpha\n");
        kprint("Site: ssdunix.xyz\n");
        kprint("Total RAM: ");
        kprint_int(get_ram_size_mb());
        kprint(" MB\n");
    }
    else if (compare_string(input, "LS") == 0) {
        list_files();
    } 
    else if (compare_string(input, "FORMAT") == 0) {
        format_disk();
    }
    else if (input[0] == 'T' && input[1] == 'O' && input[2] == 'U' && input[3] == 'C' && input[4] == 'H') {
        if (input[5] == ' ') touch_file(input + 6);
        else kprint("Usage: TOUCH <filename>\n");
    }
    else if (input[0] != '\0') {
        kprint("Unknown command. Try: HELP\n");
    }
}

// --- Главная точка входа ---

void kmain() {       
    clear_screen();
    kprint("Welcome to TESTASTERON!\n");
    kprint("AUTHOR SITE: ssdunix.xyz\n");
    
    char user_input[256];
    while (1) {
        kprint("> ");
        get_user_input(user_input); 
        execute_command(user_input);
    }
}

// --- Ввод пользователя (с поддержкой Shift) ---

void get_user_input(char* buffer) {
    int i = 0;
    int shift_pressed = 0;

    while (1) {
        unsigned char scancode = port_byte_in(0x60);

        // Нажат Shift (левый или правый)
        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = 1;
            continue;
        }
        // Отпущен Shift
        if (scancode == (0x2A | 0x80) || scancode == (0x36 | 0x80)) {
            shift_pressed = 0;
            continue;
        }

        // Игнорируем остальные события отпускания клавиш
        if (scancode & 0x80) continue;
        if (scancode == 0) continue;

        // Enter
        if (scancode == 0x1C) {
            buffer[i] = '\0';
            kprint("\n");
            while (port_byte_in(0x60) == 0x1C);
            break;
        }

        // Обработка символов
        if (scancode < 0x80) {
            char letter;
            if (shift_pressed) {
                letter = scancode_to_char_shift[(int)scancode];
            } else {
                letter = scancode_to_char[(int)scancode];
            }

            if (letter != 0) {
                buffer[i++] = letter;
                char str[2] = {letter, '\0'};
                kprint(str);
            }
            // Ждем отпускания клавиши
            while (port_byte_in(0x60) == scancode);
        }
    }
}