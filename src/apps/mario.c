/* apps/mario.c */
#include "../common.h"
#include "../drivers/screen.h"
#include "../drivers/lowlevel_io.h"
#include "apps.h"

void start_mario() {
    force_reset_terminal();

    char m_map[25][80];
    for (int r = 0; r < 25; r++) {
        for (int c = 0; c < 80; c++) {
            if (r >= 22) m_map[r][c] = 1;
            else m_map[r][c] = 0;
        }
    }

    for (int c = 15; c < 25; c++) m_map[18][c] = 2;
    for (int c = 35; c < 45; c++) m_map[14][c] = 2;
    for (int c = 55; c < 60; c++) m_map[10][c] = 2;

    m_map[20][30] = 3; m_map[21][30] = 3; m_map[20][31] = 3; m_map[21][31] = 3;
    m_map[19][65] = 3; m_map[20][65] = 3; m_map[21][65] = 3; m_map[19][66] = 3; m_map[20][66] = 3; m_map[21][66] = 3;

    m_map[16][18] = 4; m_map[16][22] = 4;
    m_map[12][40] = 4; m_map[8][57] = 4;
    m_map[21][10] = 4; m_map[21][50] = 4;

    int mx = 5, my = 21, jumping = 0, jump_power = 0, score = 0;
    unsigned long timer = 0, game_speed = 100000;
    int needs_redraw = 1;

    while (1) {
        draw_top_clock();
        timer++;

        if (timer > game_speed) {
            timer = 0;

            if (port_byte_in(0x64) & 0x01) {
                unsigned char scancode = port_byte_in(0x60);
                if (scancode == 0xE0) {
                    while ((port_byte_in(0x64) & 0x01) == 0);
                    scancode = port_byte_in(0x60);
                    if (scancode & 0x80) goto skip_input;
                } else {
                    if (scancode & 0x80) goto skip_input;
                }

                if (scancode == 0x01) break; // Выход по ESC

                if (scancode == 0x4B || scancode == 0x1E) {
                    if (mx > 0 && m_map[my][mx-1] != 1 && m_map[my][mx-1] != 2 && m_map[my][mx-1] != 3) { mx--; needs_redraw = 1; }
                }
                if (scancode == 0x4D || scancode == 0x20) {
                    if (mx < 79 && m_map[my][mx+1] != 1 && m_map[my][mx+1] != 2 && m_map[my][mx+1] != 3) { mx++; needs_redraw = 1; }
                }
                if (scancode == 0x48 || scancode == 0x11 || scancode == 0x39) {
                    if (m_map[my+1][mx] == 1 || m_map[my+1][mx] == 2 || m_map[my+1][mx] == 3) { jumping = 1; jump_power = 5; }
                }
            }
            skip_input:

            if (jumping) {
                if (jump_power > 0) {
                    if (m_map[my-1][mx] != 1 && m_map[my-1][mx] != 2 && m_map[my-1][mx] != 3) { my--; jump_power--; needs_redraw = 1; }
                    else { jumping = 0; jump_power = 0; }
                } else { jumping = 0; }
            } else {
                if (m_map[my+1][mx] == 0 || m_map[my+1][mx] == 4) { my++; needs_redraw = 1; }
            }

            if (m_map[my][mx] == 4) { score += 100; m_map[my][mx] = 0; needs_redraw = 1; }

            if (needs_redraw) {
                unsigned char *vidmem = (unsigned char *) 0xB8000;
                for (int r = 0; r < 24; r++) {
                    for (int c = 0; c < 80; c++) {
                        int offset = (r * 80 + c) * 2;
                        unsigned char ch = ' ', color = 0x0F;
                        if (r == my && c == mx) { ch = 0x02; color = 0x0C; }
                        else {
                            int map_obj = m_map[r][c];
                            if (map_obj == 1) { ch = 0xDB; color = 0x0A; }
                            else if (map_obj == 2) { ch = 0xDB; color = 0x06; }
                            else if (map_obj == 3) { ch = 0xDB; color = 0x02; }
                            else if (map_obj == 4) { ch = 0x04; color = 0x0E; }
                        }
                        vidmem[offset] = ch; vidmem[offset+1] = color;
                    }
                }

                print_at(" SUPER MARIO BROS (ASCII) ", 2, 0, 0x1F);
                print_at(" SCORE: ", 58, 0, 0x1F);
                char s_buf[10]; int t_score = score, idx = 0;
                if (t_score==0) s_buf[idx++]='0';
                while(t_score>0) { s_buf[idx++]=(t_score%10)+'0'; t_score/=10; }
                s_buf[idx]='\0';
                for(int j=0; j<idx/2; j++) { char t=s_buf[j]; s_buf[j]=s_buf[idx-j-1]; s_buf[idx-j-1]=t; }
                print_at(s_buf, 66, 0, 0x1E);
                needs_redraw = 0;
            }
        }
    }

    // ВЫХОДИМ - СБРАСЫВАЕМ ТЕРМИНАЛ
    force_reset_terminal();
}
