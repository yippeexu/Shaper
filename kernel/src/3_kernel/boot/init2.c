
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <system.h>
#include <stdio.h>

#include <shapervfs.h>
#include <serial.h>
#include <image.h>
#include <mouse.h>
#include <task.h>
#include <heap.h>
#include <vesa.h>
#include <font.h>
#include <pit.h>
#include <vga.h>
#include <x86.h>
#include <io.h>

void init2()
{
    tasking_init();

    for (;;) ;
}