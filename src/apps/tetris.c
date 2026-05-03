/* apps/tetris.c */
#include "../common.h"
#include "../drivers/screen.h"
#include "../drivers/lowlevel_io.h"
#include "apps.h"

const char t_shapes[7][17] = {
    "..X...X...X...X.", ".....XX..XX.....", "......X..XXX....",
    "......XX.XX.....", ".....XX...XX....", ".....X...XXX....", ".......X.XXX...."
};
const unsigned char t_colors[7] = {0x0B, 0x0E, 0x05, 0x0A, 0x0C, 0x09, 0x0D};
char t_board[20][10];

int t_rotate(int px, int py, int r) {
    switch (r % 4) {
        case 0: return py * 4 + px; case 1: return 12 + py - (px * 4);
        case 2: return 15 - (py * 4) - px; case 3: return 3 - py + (px * 4);
    }
    return 0;
}

int t_check(int piece, int rot, int x, int y) {
    for (int px = 0; px < 4; px++) {
        for (int py = 0; py < 4; py++) {
            if (t_shapes[piece][t_rotate(px, py, rot)] == 'X') {
                int bx = x + px, by = y + py;
                if (bx < 0 || bx >= 10 || by >= 20) return 0;
                if (by >= 0 && t_board[by][bx] != 0) return 0;
            }
        }
    }
    return 1;
}

void start_tetris() {
    force_reset_terminal();
    for(int i=0; i<20; i++) for(int j=0; j<10; j++) t_board[i][j] = 0;

    unsigned int rand_seed = BCD_TO_BIN(read_rtc_register(0x00)) + BCD_TO_BIN(read_rtc_register(0x02)) * 60;
    int piece = rand_seed % 7, rot = 0, cx = 3, cy = 0, score = 0, game_over = 0;
    unsigned long timer = 0, speed = 250000;
    int needs_redraw = 1;

    draw_window(28, 1, 22, 22, 0x0F);
    print_at(" TETRIS ", 35, 1, 0x0F); print_at("Score: 0", 54, 3, 0x0F);
    print_at("[ESC] Quit", 54, 5, 0x08); print_at("[W] Rotate", 54, 7, 0x0F);
    print_at("[A][D] Move", 54, 8, 0x0F); print_at("[S] Drop", 54, 9, 0x0F);

    while(!game_over) {
        draw_top_clock();
        timer++;
        if (timer > speed) {
            timer = 0;
            if (t_check(piece, rot, cx, cy + 1)) { cy++; needs_redraw = 1; }
            else {
                for (int px = 0; px < 4; px++) {
                    for (int py = 0; py < 4; py++) {
                        if (t_shapes[piece][t_rotate(px, py, rot)] == 'X') { if (cy + py >= 0) t_board[cy + py][cx + px] = piece + 1; }
                    }
                }
                int lines_cleared = 0;
                for (int y = 19; y >= 0; y--) {
                    int full = 1;
                    for (int x = 0; x < 10; x++) { if (t_board[y][x] == 0) full = 0; }
                    if (full) {
                        lines_cleared++;
                        for (int yy = y; yy > 0; yy--) { for (int x = 0; x < 10; x++) t_board[yy][x] = t_board[yy-1][x]; }
                        for (int x = 0; x < 10; x++) t_board[0][x] = 0;
                        y++;
                    }
                }
                if (lines_cleared) {
                    score += lines_cleared * 100;
                    char s_buf[10]; int t_score = score, idx = 0;
                    if (t_score==0) s_buf[idx++]='0';
                    while(t_score>0) { s_buf[idx++]=(t_score%10)+'0'; t_score/=10; }
                    s_buf[idx]='\0';
                    for(int j=0; j<idx/2; j++) { char t=s_buf[j]; s_buf[j]=s_buf[idx-j-1]; s_buf[idx-j-1]=t; }
                    print_at("Score:       ", 54, 3, 0x0F); print_at("Score: ", 54, 3, 0x0F); print_at(s_buf, 61, 3, 0x0A);
                }
                rand_seed = rand_seed * 1103515245 + 12345;
                piece = (rand_seed / 65536) % 7; rot = 0; cx = 3; cy = 0;
                if (!t_check(piece, rot, cx, cy)) game_over = 1;
                needs_redraw = 1;
            }
        }
        if (port_byte_in(0x64) & 0x01) {
            unsigned char scancode = port_byte_in(0x60);
            if (scancode == 0xE0) {
                while ((port_byte_in(0x64) & 0x01) == 0); scancode = port_byte_in(0x60); if (scancode & 0x80) continue;
            } else { if (scancode & 0x80) continue; }

            if (scancode == 0x01) break; // Выход по ESC
            if (scancode == 0x48 || scancode == 0x11) { if (t_check(piece, rot + 1, cx, cy)) { rot++; needs_redraw = 1; } }
            if (scancode == 0x4B || scancode == 0x1E) { if (t_check(piece, rot, cx - 1, cy)) { cx--; needs_redraw = 1; } }
            if (scancode == 0x4D || scancode == 0x20) { if (t_check(piece, rot, cx + 1, cy)) { cx++; needs_redraw = 1; } }
            if (scancode == 0x50 || scancode == 0x1F) { if (t_check(piece, rot, cx, cy + 1)) { cy++; needs_redraw = 1; timer = speed; } }
        }
        if (needs_redraw) {
            for (int y = 0; y < 20; y++) {
                for (int x = 0; x < 10; x++) {
                    unsigned char col = 0x00;
                    if (t_board[y][x] > 0) col = t_colors[t_board[y][x] - 1];
                    for (int px = 0; px < 4; px++) {
                        for (int py = 0; py < 4; py++) {
                            if (t_shapes[piece][t_rotate(px, py, rot)] == 'X') { if (cx + px == x && cy + py == y) col = t_colors[piece]; }
                        }
                    }
                    unsigned char *vidmem = (unsigned char *) 0xB8000;
                    int offset = ((y + 2) * 80 + (x * 2 + 29)) * 2;
                    if (col == 0x00) { vidmem[offset] = '.'; vidmem[offset+1] = 0x08; vidmem[offset+2] = ' '; vidmem[offset+3] = 0x08; }
                    else { vidmem[offset] = 0xDB; vidmem[offset+1] = col; vidmem[offset+2] = 0xDB; vidmem[offset+3] = col; }
                }
            }
            needs_redraw = 0;
        }
    }
    if (game_over) {
        print_at(" GAME OVER ", 33, 10, 0x4F);
        while (1) {
            if (port_byte_in(0x64) & 0x01) {
                unsigned char sc = port_byte_in(0x60); if (sc == 0x01 || sc == 0x1C) break;
            }
        }
    }

    // ВЫХОДИМ - СБРАСЫВАЕМ ТЕРМИНАЛ
    force_reset_terminal();
}
