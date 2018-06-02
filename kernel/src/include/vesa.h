
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct tag_vesa_biosinfo vesa_biosinfo_t;
struct tag_vesa_biosinfo
{
    char signature[4];
    uint16_t version;
    uint32_t oem_ptr;
    uint32_t capabilities;
    uint32_t video_modes_ptr;
    uint32_t video_memory;
    uint16_t software_revision;
    uint32_t vendor_str;
    uint32_t product_name_str;
    uint32_t product_revision_str;
    uint8_t reserved[222];
    uint8_t oem_data[256];
} __attribute__((packed));

typedef struct tag_vesa_modeinfo vesa_modeinfo_t;
struct tag_vesa_modeinfo
{
    uint16_t attributes;
    uint8_t window_a;
    uint8_t window_b;
    uint16_t granularity;
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint8_t w_char;
    uint8_t y_char;
    uint8_t planes;
    uint8_t bpp;
    uint8_t banks;
    uint8_t memory_model;
    uint8_t bank_size;
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer;
    uint32_t off_screen_mem_offset;
    uint16_t off_screen_mem_size;
    uint8_t reserved[206];
} __attribute__((packed));

uint32_t vesa_width;
uint16_t vesa_pitch;
uint32_t vesa_height;
uint32_t vesa_bpp;
uint32_t vesa_backcolor;
uint8_t *vesa_framebuffer;

void vesa_init(uint16_t width, uint16_t height, uint8_t bpp, uint32_t background_color);
bool vesa_setmode(uint16_t width, uint16_t height, uint8_t bpp);
void vesa_switchmode();

void vesa_clear(uint32_t color);
void vesa_setpixel(uint32_t x, uint32_t y, uint32_t color);
uint32_t vesa_getpixel(uint32_t x, uint32_t y);
void vesa_setpixel_overmouse(uint32_t x, uint32_t y, uint32_t color);
uint32_t vesa_getpixel_overmouse(uint32_t x, uint32_t y);
void vesa_clear_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
void vesa_draw_line(uint32_t x, uint32_t y, uint32_t width, uint32_t color);
void vesa_draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color, bool border, uint8_t border_size);
void vesa_draw_rect_to(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);
void vesa_get_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t data[]);
void vesa_draw_ellipse(uint32_t x_cnt, uint32_t y_cnt, uint32_t radius, uint32_t color, bool filled);
void vesa_draw_picture(uint32_t x, uint32_t y, uint8_t *bmp_data);
void vesa_draw_mouse(uint32_t x, uint32_t y);