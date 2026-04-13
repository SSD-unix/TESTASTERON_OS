# Пути к исходникам
C_SOURCES = $(wildcard ../kernel/*.c ../drivers/*.c ../*.c)
# Важно: kernel_entry.o должен быть ПЕРВЫМ в списке для линковки
OBJ = kernel_entry.o $(notdir $(C_SOURCES:.c=.o))

CC = i386-elf-gcc
LD = i386-elf-ld
# Добавил -Wall для отлова ошибок и -m32 на всякий случай
CFLAGS = -g -ffreestanding -std=c99 -Wall -m32

# Главная цель
os-image.bin: bootsect.bin kernel.bin
	cat bootsect.bin kernel.bin > os-image.bin
	truncate -s 1M os-image.bin

run: os-image.bin
	qemu-system-i386 -drive format=raw,file=os-image.bin,if=floppy

# Сборка загрузчика
bootsect.bin: ../boot/bootsect.asm
	nasm $< -f bin -i ../boot/ -o $@

# Линковка ядра
# Добавлен флаг -e _start, чтобы линковщик точно знал, где начало
kernel.bin: $(OBJ)
	$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary -e _start

# Правила для компиляции
kernel_entry.o: ../boot/kernel_entry.asm
	nasm $< -f elf -o $@

# Указываем VPATH, чтобы make знал, где искать .c файлы для правил %.o
VPATH = ../kernel:../drivers:..

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.bin *.o *.elf