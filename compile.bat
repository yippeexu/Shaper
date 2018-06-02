@echo off
SETLOCAL EnableDelayedExpansion
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set "DEL=%%a"
)

REM Delete kernel\Shaper iso file
echo Cleaning stuff...
rmdir /Q /S temp

REM Create Working Directory
mkdir temp

REM Compile Assembly files
echo Compiling OS Assembly files
for /r .\kernel\asm\ %%E in (*.asm) do "tools\win\nasm" -felf kernel\asm\%%~nE.asm -o temp\%%~nE.asm.o

REM Compile C files
echo Compiling OS C files
for /r .\kernel\src\ %%D in (*.c) do \mingw\bin\i686-elf-gcc -w -Ikernel\src\include -Ikernel\lib -c -ffreestanding -fomit-frame-pointer -nostdlib -g %%D -o temp\%%~nD.o

echo Linking object files
for /r %%F in (temp\*.o) do call :linkFiles %%F
C:\mingw\bin\i686-elf-gcc -ffreestanding -nostdlib -g -T linker.ld %linked_files% -o temp\Shaper.elf -lgcc

mkdir temp\iso\boot\grub
if not exist .\temp\Shaper.elf ( call :kernelFailed )
copy temp\Shaper.elf temp\iso\boot\Shaper.elf >NUL
copy temp\Shaper.elf D:\Shaper.elf >NUL
copy bootloader\grub.cfg temp\iso\boot\grub\grub.cfg >NUL
echo Creating Shaper ISO file
bash -c "grub-mkrescue /mnt/c/Users/KingLuigi4932/Documents/GitHub/Shaper/temp/iso -o /mnt/c/Users/KingLuigi4932/Documents/GitHub/Shaper/Shaper.iso" >NUL
rmdir /Q /S temp
"\Program Files (x86)\qemu\qemu-system-i386.exe" -show-cursor -netdev user,id=n0 -device pcnet,netdev=n0 -boot d -cpu SandyBridge -net nic,model=pcnet -cdrom .\Shaper.iso
"\Program Files (x86)\Bochs-2.6.9\bochsdbg.exe" -q -f vmprofiles\bochs.bxrc
"\Program Files (x86)\VMware\VMware Workstation\vmplayer.exe" vmprofiles\vmware\ShaperOS.vmx
exit

:linkFiles
set linked_files=%linked_files% %1
goto :eof

:kernelFailed
call :coloredText 0c "Kernel failed to build!"
echo.
call :coloredText 0c "Press any key to continue..."
echo.
rmdir /Q /S temp
pause >NUL
exit

:coloredText
echo off
<nul set /p ",=%DEL%" > "%~2"
findstr /v /a:%1 /R "^$" "%~2" nul
del "%~2" > nul 2>&1
goto :eof
