
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <ps2_mouse.h>
#include <mouse.h>
#include <vga.h>

void (*mousehandler)(uint32_t x, uint32_t y, bool left, bool right, bool middle);
void mouse_handler(uint32_t x, uint32_t y, bool left, bool right, bool middle);

void mouse_set(uint16_t starting_x, uint16_t starting_y, uint16_t border_w, uint16_t border_h)
{
    ps2mouse_add_handler(mouse_handler);
    ps2mouse_init(starting_x, starting_y, border_w, border_h);
    ps2mouse_draw();
}

void mouse_show()
{
    ps2mouse_show();
}

void mouse_hide()
{
    ps2mouse_hide();
}

void mouse_handler(uint32_t x, uint32_t y, bool left, bool right, bool middle)
{
    //mouse_device->update();

    if (mousehandler)
        mousehandler(x, y, left, right, middle);
}

void mouse_add_handler(void (*custom_mousehandler)(uint32_t x, uint32_t y, bool left, bool right, bool middle))
{
    mousehandler = custom_mousehandler;
}

void mouse_remove_handler()
{
    mousehandler = 0;
}