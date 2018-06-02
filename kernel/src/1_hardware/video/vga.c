
#include <sysprint.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include <ps2_mouse.h>
#include <vga.h>
#include <io.h>

#define MODE13H_OFFSET 0
#define MODE3H_OFFSET 30
#define MODE_X_H_OFFSET 60
#define MODE12H_OFFSET 90

uint8_t *img_dat = 0x80F000;
uint8_t *vga_buffer = 0xA0000;
vgamode_t current_vgamode;
uint8_t background;
void vga_write_settings(uint8_t* regs);
uint8_t *_24bpp_to_8bpp(uint32_t *_24bpp_data, uint32_t length);
uint8_t modes_regsettings[];
uint32_t mouse_palettes[];
uint8_t mouse_imgdata[];

uint32_t vga_width;
uint32_t vga_height;
uint32_t vga_bpp;
bool vga_planar;

bool mouse_drawn;
uint32_t mouse_xstart;
uint32_t mouse_ystart;
uint32_t mouse_xend;
uint32_t mouse_yend;

void vga_write_settings(uint8_t* regs)
{ 
    // Misc Output Reg
    outb(0x3C2, regs[5]);

    // Clock Mode Register
    outb(0x3C4, 0x01);
    outb(0x3C5, regs[6]);
    
    // Character select
    outb(0x3C4, 0x03);
    outb(0x3C5, regs[7]);

    // Memory mode Register
    outb(0x3C4, 0x04);
    outb(0x3C5, regs[8]);
    
    // Unlock CRTC Registers
    outb(0x3D4, 0x03);
    outb(0x3D5, inb(0x3D5) | 0x80);

    outb(0x3D4, 0x11);
    outb(0x3D5, inb(0x3D5) | 0x7F);

    regs[0x03] |= 0x80;
    regs[0x11] &= 0x7F;

    // Horizontal Total
    outb(0x3D4, 0x00);
    outb(0x3D5, regs[11]);

    // Horizontal Display Enable End
    outb(0x3D4, 0x01);
    outb(0x3D5, regs[12]);

    // Horizontal Blank Start
    outb(0x3D4, 0x02);
    outb(0x3D5, regs[13]);

    // Horizontal Blank End
    outb(0x3D4, 0x03);
    outb(0x3D5, regs[14]);
    
    // Horizontal Retrace Start
    outb(0x3D4, 0x04);
    outb(0x3D5, regs[15]);
    
    // Horizontal Retrace End
    outb(0x3D4, 0x05);
    outb(0x3D5, regs[16]);

    // Vertical Total
    outb(0x3D4, 0x06);
    outb(0x3D5, regs[17]);

    // Overflow register
    outb(0x3D4, 0x07);
    outb(0x3D5, regs[18]);

    // Preset row scan
    outb(0x3D4, 0x08);
    outb(0x3D5, regs[19]);

    // Maximum Scan Line
    outb(0x3D4, 0x09);
    outb(0x3D5, regs[20]);    

    // Vertical Retrace Start
    outb(0x3D4, 0x10);
    outb(0x3D5, regs[21]);
    
    // Vertical Retrace End
    outb(0x3D4, 0x11);
    outb(0x3D5, regs[22]);

    // Vertical Display Enable End
    outb(0x3D4, 0x12);
    outb(0x3D5, regs[23]);
    
    // Logical Width
    outb(0x3D4, 0x13);
    outb(0x3D5, regs[24]);

    // Underline Location
    outb(0x3D4, 0x14);
    outb(0x3D5, regs[25]);

    // Vertical Blank Start
    outb(0x3D4, 0x15);
    outb(0x3D5, regs[26]);
    
    // Vertical Blank End
    outb(0x3D4, 0x16);
    outb(0x3D5, regs[27]);

    // Mode Control
    outb(0x3D4, 0x17);
    outb(0x3D5, regs[28]);
    
    // Mode Register
    outb(0x3CE, 0x05);
    outb(0x3CF, regs[9]);

    // Misc Register
    outb(0x3CE, 0x06);
    outb(0x3CF, regs[10]);
    
    // Mode control
    inb(0x3DA);
    outb(0x3C0, 0x10);
    outb(0x3C0, regs[0]);
    inb(0x3DA);
    
    // Overscan register
    inb(0x3DA);
    outb(0x3C0, 0x11);
    outb(0x3C0, regs[1]);
    inb(0x3DA);
    
    // Color Plane Enable
    inb(0x3DA);
    outb(0x3C0, 0x12);
    outb(0x3C0, regs[2]);
    inb(0x3DA);
    
    // Horizontal Panning
    inb(0x3DA);
    outb(0x3C0, 0x13);
    outb(0x3C0, regs[3]);
    inb(0x3DA);
    
    // Color select
    inb(0x3DA);
    outb(0x3C0, 0x14);
    outb(0x3C0, regs[4]);
    inb(0x3DA);

    inb(0x3DA);
    outb(0x3C0, 0x20);
}

void vga_init(vgamode_t vga_mode, cga_color_t color)
{
    vga_setmode(vga_mode);
    background = color;
}

void vga_setmode(vgamode_t vga_mode)
{
    current_vgamode = vga_mode;
}

void vga_switchmode()
{
    switch(current_vgamode)
    {
    case VGAMODE_320x200x256:
        vga_write_settings(modes_regsettings + MODE13H_OFFSET);
        vga_width = 320;
        vga_height = 200;
        vga_bpp = 8;
        vga_planar = false;
        memset(vga_buffer, background, 320*200);
        break;
    case TEXTMODE_80x25:
        vga_write_settings(modes_regsettings + MODE3H_OFFSET);
        vga_planar = false;
        break;
    case VGAMODE_320x240x256:
        vga_write_settings(modes_regsettings + MODE_X_H_OFFSET);
        vga_width = 320;
        vga_height = 240;
        vga_bpp = 8;
        vga_planar = true;
        for (int i = 0; i < 4; i++)
        {
            outb(0x3C4, 0x02);
            outb(0x3C5, 1 << i);
            memset(vga_buffer, background, 19200);
        }
        break;
    case VGAMODE_640x480x16:
        vga_write_settings(modes_regsettings + MODE12H_OFFSET);
        vga_width = 640;
        vga_height = 480;
        vga_bpp = 4;
        vga_planar = true;
        for (int i = 0; i < 4; i++)
        {
            outb(0x3C4, 0x02);
            outb(0x3C5, 1 << i);
            memset(vga_buffer, background, 19200);
        }
        break;
    };

    vga_write_palette(0xF0, mouse_palettes, 6);
}

void vga_setpixel(uint32_t x, uint32_t y, cga_color_t color)
{
    if (x >= vga_width || y >= vga_height) return;
    
    if (mouse_drawn)
        if (x >= mouse_xstart && y >= mouse_ystart && x <= mouse_xend && y <= mouse_yend && mouse_imgdata[(y-mouse_ystart)*10+(x-mouse_xstart)])
        {
            back_data[(y-mouse_ystart)*10+(x-mouse_xstart)] = color;
            return;
        }

    if (!vga_planar) {
        uint32_t offset = y * 320 + x;
        vga_buffer[offset] = color;
    } else {
        uint32_t offset = y * 320 + x;
        outb(0x3C4, 0x02);
        outb(0x3C5, 1 << (offset%4));
        vga_buffer[offset/4] = color;
    }
}

void vga_setpixel_overmouse(uint32_t x, uint32_t y, cga_color_t color)
{
    if (x >= vga_width || y >= vga_height) return;

    if (!vga_planar) {
        uint32_t offset = y * 320 + x;
        vga_buffer[offset] = color;
    } else {
        uint32_t offset = y * 320 + x;
        outb(0x3C4, 0x02);
        outb(0x3C5, 1 << (offset%4));
        vga_buffer[offset/4] = color;
    }
}

cga_color_t vga_getpixel(uint32_t x, uint32_t y)
{
    if (x >= vga_width || y >= vga_height) return;
    
    if (mouse_drawn)
        if (x >= mouse_xstart && x <= mouse_xend && y >= mouse_ystart && y <= mouse_yend && mouse_imgdata[(y-mouse_ystart)*10+(x-mouse_xstart)])
            return back_data[(y-mouse_ystart)*10+(x-mouse_xstart)];

    if (!vga_planar) {
        uint32_t offset = y * 320 + x;
        return vga_buffer[offset];
    } else {
        uint32_t offset = y * 320 + x;
        outb(0x3CE, 0x04);
        outb(0x3CF, (offset%4));
        return vga_buffer[offset/4];
    }
}

cga_color_t vga_getpixel_overmouse(uint32_t x, uint32_t y)
{
    if (x >= vga_width || y >= vga_height) return;
    
    if (!vga_planar) {
        uint32_t offset = y * 320 + x;
        return vga_buffer[offset];
    } else {
        uint32_t offset = y * 320 + x;
        outb(0x3CE, 0x04);
        outb(0x3CF, (offset%4));
        return vga_buffer[offset/4];
    }
}

void vga_draw_line(uint32_t x, uint32_t y, uint32_t width, cga_color_t color)
{
    for (int i = 0; i < width; i++)
    {
        vga_setpixel(x+i, y, color);
    }
}

void vga_draw_ellipse(uint32_t x_cnt, uint32_t y_cnt, uint32_t radius, cga_color_t color, bool filled)
{
    int16_t x = radius-1;
    int16_t y = 0;
    int16_t dx = 1;
    int16_t dy = 1;
    int16_t err = dx - (radius << 1);
    while (x >= y)
    {
        if (filled)
        {
            vga_draw_rect_from(x_cnt - x, y_cnt + y, x_cnt + x, y_cnt + y, color);
            vga_draw_rect_from(x_cnt - x, y_cnt - y, x_cnt + x, y_cnt - y, color);
            vga_draw_rect_from(x_cnt - y, y_cnt + x, x_cnt + y, y_cnt + x, color);
            vga_draw_rect_from(x_cnt - y, y_cnt - x, x_cnt + y, y_cnt - x, color);
        }
        else
        {
            vga_setpixel(x_cnt + x, y_cnt + y, color);
            vga_setpixel(x_cnt - x, y_cnt + y, color);
            vga_setpixel(x_cnt + x, y_cnt - y, color);
            vga_setpixel(x_cnt - x, y_cnt - y, color);
            vga_setpixel(x_cnt + y, y_cnt + x, color);
            vga_setpixel(x_cnt - y, y_cnt + x, color);
            vga_setpixel(x_cnt + y, y_cnt - x, color);
            vga_setpixel(x_cnt - y, y_cnt - x, color);    
        }
    
        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void vga_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, cga_color_t color, bool border, uint8_t border_size)
{
    if (border) {
        for (int t = 0; t < border_size; t++)
        {
            vga_draw_line(x+1, y+t, width-1, color);
        }
        for (int i = 0; i < height; i++)
        {
            for (int t = 0; t < border_size; t++)
            {
                vga_setpixel(x+t, y+i, color);
                vga_setpixel(x+width-border_size+t, y+i, color);
            }
        }
        for (int t = 0; t < border_size; t++)
        {
            vga_draw_line(x+1, y+height-border_size+t, width-1, color);
        }
    } else {
        for (int i = 0; i < height; i++)
        {
            vga_draw_line(x, y+i, width, color);
        }
    }
}

void vga_draw_rect_from(uint32_t x, uint32_t y, uint32_t x_to, uint32_t y_to, cga_color_t color)
{
    for (int i = 0; i < y_to-y+1; i++)
    {
        vga_draw_line(x, y, x_to-x+1, color);
    }
}

void vga_get_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t data[])
{
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            data[i*width+j] = vga_buffer[y+i*vga_width+x+j];
}

// Supports BMP only!
void vga_draw_picture(uint32_t x, uint32_t y, uint8_t *bmp_data)
{
    if (bmp_data[0] != 'B' && bmp_data[1] != 'M') {
        printstr("Invaild BMP");
        return;
    }

    uint32_t *imgdat_offset = &bmp_data[10];
    uint32_t *img_width = &bmp_data[18];
    uint32_t *img_height = &bmp_data[22];
    uint32_t *colors_count = &bmp_data[46];
    for (int i = 0; i < *img_width**img_height; i++)
        img_dat[i] = bmp_data[*imgdat_offset+i];

    uint32_t palette[] =
    {
        0x000000/4,
        0x141338/4,
        0x212135/4,
        0x2E2E49/4,
        0x557AE0/4,
        0x9794EF/4
    };// = &bmp_data[54];

    vga_write_palette(0x40, palette, 192);

    for (int img_y = *img_height-1; img_y >= 0; img_y--)
    {
        for (int img_x = 0; img_x < *img_width; img_x++)
        {
            if (palette[img_dat[img_y**img_width+img_x]])
                vga_setpixel(x+img_x, y+*img_height-1-img_y, 0x40+img_dat[img_y**img_width+img_x]);
        }
    }
}

void vga_write_palette(uint8_t color_index_start, uint32_t *colors, uint32_t colors_count)
{
    outb(0x3C8, color_index_start);
    for (int i = 0; i < colors_count; i++)
    {
        outb(0x3C9, ((colors[i] >> 16) & 0xFF));
        outb(0x3C9, ((colors[i] >> 8) & 0xFF));
        outb(0x3C9, (colors[i] & 0xFF));
    }
}

uint8_t *_24bpp_to_8bpp(uint32_t *_24bpp_data, uint32_t length)
{
    uint8_t *_8bpp_colors;
    for (int i = 0; i < length; i++) {
        uint8_t red = (_24bpp_data[i] & 0xFF);
        uint8_t green = (_24bpp_data[i] >> 8) & 0xFF;
        uint8_t blue = (_24bpp_data[i] >> 16) & 0xFF;
        cga_color_t _8bpp_color = (red*6/256)*36 + (green*6/256)*6 + (blue*6/256);
        _8bpp_colors[i] = _8bpp_color;
    }
}

void vga_draw_chr(uint8_t *chr_data, size_t count)
{
    for (int e = 0; e < count; e++)
    {
        uint64_t *chr = &chr_data[e*16];
        for (int i = 0; i < 64; i++)
            if ((chr[0] >> i) & 1)
                vga_setpixel(i%8, i/8, 0x3F);
        
        //for (int i = 0; i < 64; i++)
        //    if ((chr[1] >> i) & 1)
        //        vga_setpixel(i%8, i/8, 0x3F);
    }
}

void vga_clear_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    for (int j = y; j < j+height; j++)
        for (int i = x; i < i+width; i++)
            vga_setpixel(i, j, background);

    //vga_draw_rect(x, y, width, height, background);
}

void vga_draw_mouse(uint32_t x, uint32_t y)
{
    mouse_drawn = true;
    mouse_xstart = x;
    mouse_ystart = y;
    mouse_xend = x+9;
    mouse_yend = x+15;

    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 10; j++)
            back_data[i*10+j] = vga_getpixel_overmouse(x+j, y+i);

    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 10; j++)
            if (mouse_imgdata[i*10+j])
                vga_setpixel_overmouse(x+j, y+i, 0xF0+mouse_imgdata[i*10+j]);
            //else vga_setpixel_overmouse(x+j, y+i, back_data[i*10+j]);
}

void vga_switch_buffer()
{
    if (vga_buffer == 0xA0000) {
        memcpy(0xA0000, 0xF8000, 320*200);
        vga_buffer = 0xF8000;
    } else {
        memcpy(0xA0000, 0xF8000, 320*200);
        vga_buffer = 0xA0000;
    }
}

uint32_t mouse_palettes[] =
{
    0x000000/4,
    0x141338/4,
    0x212135/4,
    0x2E2E49/4,
    0x557AE0/4,
    0x9794EF/4
};

uint8_t mouse_imgdata[] =
{
    03, 00, 00, 00, 00, 00, 00, 00, 00, 00,
    03, 03, 00, 00, 00, 00, 00, 00, 00, 00,
    03, 01, 03, 00, 00, 00, 00, 00, 00, 00, 
    03, 01, 01, 03, 00, 00, 00, 00, 00, 00, 
    03, 01, 01, 01, 03, 00, 00, 00, 00, 00,
    03, 01, 01, 01, 01, 03, 00, 00, 00, 00, 
    03, 01, 01, 01, 01, 01, 03, 00, 00, 00, 
    03, 01, 01, 01, 01, 01, 01, 03, 00, 00, 
    03, 01, 01, 01, 05, 05, 05, 05, 03, 00, 
    03, 01, 01, 05, 05, 05, 05, 05, 05, 03, 
    03, 01, 05, 05, 05, 05, 02, 02, 02, 02, 
    03, 05, 05, 02, 04, 04, 02, 00, 00, 00, 
    03, 05, 02, 00, 02, 04, 04, 02, 00, 00, 
    03, 02, 00, 00, 00, 02, 04, 04, 02, 00, 
    00, 00, 00, 00, 00, 02, 04, 04, 02, 00,
    00, 00, 00, 00, 00, 00, 02, 02, 00, 00
};

uint8_t modes_regsettings[] =
{
    // Mode 13H
    0x41, 0x00, 0x0F, 0x00, 0x00, 0x63, 0x01, 0x00, 0x0E, 0x40,
    0x05, 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 0x00, 
    0x41, 0x9C, 0x8E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3, 0x00,

    // Mode 3H
    0x04, 0x00, 0x0F, 0x08, 0x00, 0x67, 0x00, 0x00, 0x07, 0x10,
    0x0E, 0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F, 0x00,
    0x4F, 0x9C, 0x8E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3, 0x00,

    // Mode XH
    0x41, 0x00, 0x0F, 0x00, 0x00, 0xE3, 0x01, 0x00, 0x06, 0x40,
    0x05, 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0D, 0x3E, 0x00,
    0x41, 0xEA, 0xAC, 0xDF, 0x28, 0x00, 0xE7, 0x06, 0xE3, 0x00,

    
    /* MISC */
    0x01, 0x00, 0x0F, 0x00, 0x00, 0xE3, 0x01, 0x00, 0x02, 0x00,
    0x05, 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E, 0x00,
    0x40, 0xEA, 0x8C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3, 0x00,
};

//uint8_t vga_256_standard[] =
//{
//    0x00, 0x00, 0x00, 
//    0x80, 0x00, 0x00,
//    0x80, 0x80, 0x00,
//    0x00, 
//};