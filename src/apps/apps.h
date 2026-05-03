/* apps/apps.h */
#ifndef APPS_H
#define APPS_H

// Макрос для работы со временем BIOS (ОБЯЗАТЕЛЬНО!)
#define BCD_TO_BIN(val) ((((val) & 0xF0) >> 4) * 10 + ((val) & 0x0F))

// Системные функции из ядра
extern unsigned char read_rtc_register(unsigned char reg);
extern void force_reset_terminal();
extern void draw_top_clock();
extern void draw_window(int col, int row, int width, int height, unsigned char color);
extern void fill_rect(int col, int row, int width, int height, unsigned char color);
extern void print_at(char* str, int col, int row, unsigned char color);
extern int get_ram_size_mb();

// Наши программы
extern void start_tetris();
extern void start_mario();
extern void start_ux();


#endif
