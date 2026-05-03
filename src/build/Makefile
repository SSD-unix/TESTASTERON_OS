C_SOURCES = $(wildcard ../kernel/*.c ../drivers/*.c ../apps/*.c ../*.c)
OBJ = kernel_entry.o $(notdir $(C_SOURCES:.c=.o))

CC = i386-elf-gcc
LD = i386-elf-ld
CFLAGS = -g -ffreestanding -std=c99 -Wall -m32

# =========================================================
# ГЛАВНАЯ ЦЕЛЬ: СБОРКА LIMINE ISO
# =========================================================
testosterone.iso: kernel.bin
	@echo "=> Создаем структуру ISO..."
	mkdir -p isodir/boot/limine
	cp kernel.bin isodir/boot/

	@echo "=> Копируем бинарники Limine..."
	cp /usr/share/limine/limine-bios.sys isodir/boot/limine/
	cp /usr/share/limine/limine-bios-cd.bin isodir/boot/limine/
	cp /usr/share/limine/limine-uefi-cd.bin isodir/boot/limine/

	@echo "=> Создаем limine.conf (Новый синтаксис)..."
	echo 'timeout: 3' > isodir/boot/limine/limine.conf
	echo '/TESTOSTERONE OS' >> isodir/boot/limine/limine.conf
	echo 'protocol: multiboot1' >> isodir/boot/limine/limine.conf
	echo 'kernel_path: boot():/boot/kernel.bin' >> isodir/boot/limine/limine.conf

	@echo "=> Собираем ISO образ..."
	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		isodir -o testosterone.iso

	@echo "=> Устанавливаем Limine MBR на ISO..."
	limine bios-install testosterone.iso
	rm -rf isodir
	@echo "=> ГОТОВО! Образ testosterone.iso собран."

# =========================================================
# ЗАПУСК И КОМПИЛЯЦИЯ
# =========================================================
run: testosterone.iso
	qemu-system-i386 -cdrom testosterone.iso -m 128M

kernel.bin: $(OBJ)
	$(LD) -T ../linker.ld -o $@ $^

kernel_entry.o: ../boot/kernel_entry.asm
	nasm $< -f elf -o $@

VPATH = ../kernel:../drivers:../apps:..

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.bin *.o *.elf *.iso
	rm -rf isodir
