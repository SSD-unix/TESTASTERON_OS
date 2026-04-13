/* common.h */
#ifndef COMMON_H
#define COMMON_H

// 1. Базовые типы
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef char           int8_t;
typedef short          int16_t;
typedef int            int32_t;

// 2. Твои сокращения (чтобы kmain и остальное не падало)
typedef int32_t  s32;
typedef uint32_t u32;
typedef int16_t  s16;
typedef uint16_t u16;
typedef int8_t   s8;
typedef uint8_t  u8;

void memory_copy(char *source, char *dest, int nbytes);

#endif