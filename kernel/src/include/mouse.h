
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifndef $_MOUSE_H
#define $_MOUSE_H

void mouse_set(uint16_t starting_x, uint16_t starting_y, uint16_t border_w, uint16_t border_h);
void mouse_show();
void mouse_hide();
void mouse_add_handler(void (*custom_mousehandler)(uint32_t x, uint32_t y, bool left, bool right, bool middle));
void mouse_remove_handler();

#endif