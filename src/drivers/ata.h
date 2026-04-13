/* drivers/ata.h */
#ifndef ATA_H
#define ATA_H

#include "../common.h"

// Объявляем функции, чтобы fs.c мог их видеть
void read_sector(uint32_t lba, uint8_t* buffer);
void write_sector(uint32_t lba, uint8_t* buffer);

#endif