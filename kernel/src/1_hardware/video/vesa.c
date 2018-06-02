
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <system.h>

#include <ps2_mouse.h>
#include <encoding.h>
#include <heap.h>
#include <vesa.h>

#define vesa_offsetof_pixel(x, y) (uint32_t)(vesa_framebuffer + (y * vesa_pitch) + (x * (vesa_bpp / 8)))

uint16_t vesa_modeid;

void vesa_init(uint16_t width, uint16_t height, uint8_t bpp, uint32_t background_color)
{
    if (!vesa_setmode(width, height, bpp))
        vesa_setmode(800, 600, 24);

    vesa_backcolor = background_color;
}

bool vesa_setmode(uint16_t width, uint16_t height, uint8_t bpp)
{
    static regs16_t regs;
    vesa_modeinfo_t *modeinfo = 0xE000;
    if ((width == 1024 && height == 768) && (bpp == 24 || bpp == 32))
    {
        regs.ax = 0x4F01;
        regs.cx = 0x0118;
        regs.es = 0x0000;
        regs.di = 0x9000;
        _sys_bioscalls32(0x10, &regs);
        vesa_width = modeinfo->width;
        vesa_height = modeinfo->height;
        vesa_pitch = modeinfo->pitch;
        vesa_bpp = modeinfo->bpp;
        vesa_framebuffer = modeinfo->framebuffer;
        vesa_modeid = 0x118;
        return true;
    }
    regs.ax = 0x4F00;
    regs.es = 0x0000;
    regs.di = 0x8000;
    _sys_bioscalls32(0x10, &regs);

    uint32_t seg_off = ((vesa_biosinfo_t *)0x8000)->video_modes_ptr;
    int i = 0;
    for (; ((uint16_t *)(uint32_t)((seg_off >> 16) * 16 + (seg_off & 0xFFFF)))[i] != 0xFFFF; i++) ;
    uint16_t *supported_modes = memalloc(i*2);
    memcpy(supported_modes, (seg_off >> 16) * 16 + (seg_off & 0xFFFF), i*2);
    i = 0;
    while (true)
    {
        if (supported_modes[i] == 0xFFFF) break;
        regs.ax = 0x4F01;
        regs.cx = supported_modes[i];
        regs.es = 0x0000;
        regs.di = 0x9000;
        regs16_t *output_regs = _sys_bioscalls32(0x10, &regs);
        //if (output_regs->ax != 0x4F00) return false;
        printstr("VESA Mode ");
        printhex(supported_modes[i], 0);
        printstr(" : Screen Resolution ");
        printnum(modeinfo->width);
        printchr('x');
        printnum(modeinfo->height);
        printchr('x');
        printnum(modeinfo->bpp);
        printchr('\n');
        if (modeinfo->width == width && modeinfo->height == height && modeinfo->bpp == bpp)
        {
            vesa_width = modeinfo->width;
            vesa_height = modeinfo->height;
            vesa_pitch = modeinfo->pitch;
            vesa_bpp = modeinfo->bpp;
            vesa_framebuffer = modeinfo->framebuffer;
            vesa_modeid = supported_modes[i];
            return true;
        }
        i++;
    }
    return false;
}

void vesa_switchmode()
{
    regs16_t regs;
    regs.ax = 0x4F02;
    regs.bx = 0x4000 | vesa_modeid;
    _sys_bioscalls32(0x10, &regs);
    vesa_clear(vesa_backcolor);
}

void vesa_clear(uint32_t color)
{
    memset(vesa_framebuffer, color & 0xFFFFFF, vesa_pitch * vesa_height * vesa_bpp / 8);
}

void vesa_setpixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (x >= vesa_width || y >= vesa_height) return;

    ((uint8_t *)vesa_offsetof_pixel(x, y))[0] = color & 0xFF;
    ((uint8_t *)vesa_offsetof_pixel(x, y))[1] = color >> 8 & 0xFF;
    ((uint8_t *)vesa_offsetof_pixel(x, y))[2] = color >> 16 & 0xFF;
}

uint32_t vesa_getpixel(uint32_t x, uint32_t y)
{
    if (x >= vesa_width || y >= vesa_height) return;

    return ((uint8_t *)vesa_offsetof_pixel(x, y))[0] << 16 | ((uint8_t *)vesa_offsetof_pixel(x, y))[1] << 8 | ((uint8_t *)vesa_offsetof_pixel(x, y))[2];
}