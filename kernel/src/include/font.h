
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifndef $_FONT_H
#define $_FONT_H

typedef struct tag_font
{
    uint8_t glyph_w;
    uint8_t glyph_h;
    uint8_t *data;
} font_t;

font_t create_font(uint8_t glyph_width, uint8_t glyph_height, uint8_t *font_data);

#endif