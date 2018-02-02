AS=nasm
ARCH=i686
LD=$(ARCH)-elf-ld
CC=$(ARCH)-elf-gcc

BOOTPREFIX := ../bootloader

C_SOURCES := $(wildcard ShaperSources/*.c)
ASM_SOURCES := $(wildcard ShaperAsm/*.asm)

C_OBJS := $(patsubst ShaperSources/%.c, \
	Build/%.o, $(C_SOURCES))
ASM_OBJS := $(patsubst ShaperAsm/%.asm, \
	Build/%.o, $(ASM_SOURCES))

INCLUDE := ShaperSource/include
ASM_FLAGS := -felf32 -Fdwarf
C_FLAGS := -c -ffreestanding -nostdlib -fomit-frame-pointer -g
LINKER_FLAGS := -nostdlib -nostartfiles -Tlink.ld

all: Shaper.bin

Build/%.o: ShaperAsm/%.asm
	$(AS) $(ASM_FLAGS) $< -o $@

Build/%.o: ShaperSources/%.c
	$(CC) -I$(INCLUDE) $(CFLAGS) $< -o $@

Shaper.bin:
	$(AS) $(BOOTPREFIX)/bootcode.asm -fbin -o Build/bootcode.bin
	$(AS) $(BOOTPREFIX)/loader.asm $(ASM_FLAGS) -o Build/loader.o
	$(LD) $(LINKER_FLAGS) $(C_OBJS) $(ASM_OBJS) -o Shaper.elf
	objcopy -Obinary Shaper.elf Shaper.bin

