/* drivers/ata.c */
#include "ata.h"
#include "lowlevel_io.h"
// ... тут твой код функций read_sector и write_sector ...
/* drivers/ata.c */
#include "../common.h"
#include "lowlevel_io.h"

void read_sector(uint32_t lba, uint8_t* buffer) {
    port_byte_out(0x1F6, (lba >> 24) | 0xE0);
    port_byte_out(0x1F2, 1);
    port_byte_out(0x1F3, (uint8_t)lba);
    port_byte_out(0x1F4, (uint8_t)(lba >> 8));
    port_byte_out(0x1F5, (uint8_t)(lba >> 16));
    port_byte_out(0x1F7, 0x20);

    while (!(port_byte_in(0x1F7) & 0x08));

    int i;
    for (i = 0; i < 256; i++) {
        uint16_t data = port_word_in(0x1F0);
        buffer[i*2] = (uint8_t)data;
        buffer[i*2 + 1] = (uint8_t)(data >> 8);
    }
}

void write_sector(uint32_t lba, uint8_t* buffer) {
    port_byte_out(0x1F6, (lba >> 24) | 0xE0);
    port_byte_out(0x1F2, 1);
    port_byte_out(0x1F3, (uint8_t)lba);
    port_byte_out(0x1F4, (uint8_t)(lba >> 8));
    port_byte_out(0x1F5, (uint8_t)(lba >> 16));
    port_byte_out(0x1F7, 0x30);

    while (!(port_byte_in(0x1F7) & 0x08));

    int i;
    for (i = 0; i < 256; i++) {
        uint16_t data = buffer[i*2] | (buffer[i*2 + 1] << 8);
        port_word_out(0x1F0, data);
    }
}