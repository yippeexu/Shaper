@echo off

set AS=wintools\nasm
set ARCH=i686
set LD=\mingw\bin\%ARCH%-elf-ld
set CC=\mingw\bin\%ARCH%-elf-gcc.
set OC=\mingw\bin\objcopy
set PC=wintools\sfk partcopy
set VM="\Program Files (x86)\Bochs-2.6.9\bochs.exe"

set C_SOURCES=kernel\src\*.c
set ASM_SOURCES=kernel\asm\*.asm

set INCLUDE=kernel\src\include
set ASM_FLAGS=-felf32 -Fdwarf
set BL_FLAGS=-fbin
set C_FLAGS=-c -ffreestanding -nostdlib -fomit-frame-pointer -g
set LINKER_FLAGS=-nostdlib -nostartfiles -Tkernel\link.ld

rmdir /S /Q kernel\obj

:: Build kernel source
mkdir kernel\obj
for /r kernel\src\ %%C in (*.c) do %CC% -I%INCLUDE% %C_FLAGS% %%C -o kernel\obj\%%~nC.o
for /r kernel\asm\ %%A in (*.asm) do %AS% %ASM_FLAGS% %%A -o kernel\obj\%%~nA-asm.o
for /r kernel\obj\ %%F in (*.o) do call :GATHER_OBJS %%F

:: Build bootloader
%AS% bootloader\src\bootcode.asm %BL_FLAGS% -o bootloader\obj\bootcode.bin
%AS% bootloader\src\loader.asm %BL_FLAGS% -o bootloader\obj\loader.bin

:: Build kernel
%AS% kernel\boot.asm %ASM_FLAGS% -o kernel\obj\boot.o
%LD% %LINKER_FLAGS% %OBJS% -o disk\shaper.elf
%OC% -Obinary disk\shaper.elf disk\shaper.bin

if not exist disk\shaperos.img (
	:: Create Bootable Shaper floppy disk
	imdisk -a -s 1440K -m A: -f disk\shaperos.img -p "/FS:FAT /Y"
) else (
	:: Replace files
	imdisk -a -s 1440K -m A: -f disk\shaperos.img
)
copy disk\shaper.bin A:\shaper.bin
copy bootloader\obj\loader.bin A:\loader.bin
imdisk -D -m A:
%PC% bootloader\obj\bootcode.bin 0 512 disk\shaperos.img 0 -yes 

:: Run a virtual machine
echo Continue to run Shaper on a virtual macine!
pause
%VM% -q -f vmprofiles\bochs.bxrc
exit

:GATHER_OBJS
set OBJS=%OBJS% %1
goto :eof
