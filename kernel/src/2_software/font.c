
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <font.h>

font_t create_font(uint8_t glyph_width, uint8_t glyph_height, uint8_t *font_data)
{
    font_t new_font;
    new_font.glyph_w = glyph_width;
    new_font.glyph_h = glyph_height;
    new_font.data = font_data;

    return new_font;
}