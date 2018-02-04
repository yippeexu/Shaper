AS=nasm
ARCH=i686
LD=$(ARCH)-elf-ld
CC=$(ARCH)-elf-gcc

C_SOURCES := $(wildcard kernel/src/*.c)
ASM_SOURCES := $(wildcard kernel/asm/*.asm)

C_OBJS := $(patsubst kernel/src/%.c, \
	 kernel/obj/%.o, $(C_SOURCES))
ASM_OBJS := $(patsubst kernel/asm/%.asm, \
	 kernel/obj/%-asm.o, $(ASM_SOURCES))

INCLUDE := kernel/src/include
ASM_FLAGS := -felf32 -Fdwarf
BL_FLAGS := -fbin
C_FLAGS := -c -ffreestanding -nostdlib -fomit-frame-pointer -g
LINKER_FLAGS := -nostdlib -nostartfiles -Tlink.ld

all: Shaper.bin

kernel/obj/%.o: kernel/asm/%.asm
	$(AS) $(ASM_FLAGS) $< -o $@

kernel/obj/%.o: kernel/src/%.c
	$(CC) -I$(INCLUDE) $(C_FLAGS) $< -o $@

Shaper.bin:
	$(AS) bootloader/src/bootcode.asm $(BL_FLAGS) -o kernel/obj/bootcode.bin
	$(AS) bootloader/src/loader.asm $(BL_FLAGS) -o kernel/obj/loader.bin
	$(LD) $(LINKER_FLAGS) $(C_OBJS) $(ASM_OBJS) -o disk/shaper.elf
	objcopy -Obinary disk/shaper.elf disk/shaper.bin

	# Copy 
	dd bs=512 count=2880 if=/dev/zero of=disk/shaperos.img
	mkfs.msdos disk/shaperos.img
	sudo mount -o loop disk/shaperos.img /media/floppy/
	cp disk/shaper.bin /media/floppy/shaper.bin
	cp bootloader/obj/loader.bin /media/floppy/loader.bin
	sudo umount /media/floppy/
	dd if=bootloader/obj/bootcode.bin of=disk/shaperos.img conv=notrunc
	
