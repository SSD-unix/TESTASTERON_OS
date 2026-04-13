/* kernel/fs.c */
#include "../common.h"
#include "../drivers/ata.h"
#include "utils.h"
#include "../drivers/screen.h"

#define FS_ROOT_SECTOR 30

struct file_entry {
    char name[12];
    uint32_t start_sector;
    uint32_t size;
    uint8_t active;
};

// Подготовка диска
void format_disk() {
    uint8_t buffer[512];
    for(int i = 0; i < 512; i++) buffer[i] = 0;
    
    write_sector(FS_ROOT_SECTOR, buffer);
    kprint("File system initialized at sector 30.\n");
}

// Список файлов
void list_files() {
    struct file_entry dir[16]; // 16 записей влезают в 1 сектор (512 байт)
    read_sector(FS_ROOT_SECTOR, (uint8_t*)dir);

    int found = 0;
    for (int i = 0; i < 16; i++) {
        if (dir[i].active == 1) {
            kprint("- ");
            kprint(dir[i].name);
            kprint("\n");
            found = 1;
        }
    }
    if (!found) kprint("No files found. Try FORMAT first.\n");
}

// Создание пустого файла
void touch_file(char* filename) {
    struct file_entry dir[16];
    read_sector(FS_ROOT_SECTOR, (uint8_t*)dir);

    for (int i = 0; i < 16; i++) {
        if (dir[i].active == 0) {
            // Копируем имя
            int j = 0;
            while(filename[j] != '\0' && j < 11) {
                dir[i].name[j] = filename[j];
                j++;
            }
            dir[i].name[j] = '\0';
            
            dir[i].active = 1;
            dir[i].start_sector = FS_ROOT_SECTOR + 1 + i;
            dir[i].size = 0;

            write_sector(FS_ROOT_SECTOR, (uint8_t*)dir);
            kprint("File '");
            kprint(dir[i].name);
            kprint("' created.\n");
            return;
        }
    }
    kprint("Error: Directory full.\n");
}