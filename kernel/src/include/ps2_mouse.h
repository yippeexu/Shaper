
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

bool mouseirq_handled;
bool mouse_leftbutton;
bool mouse_middlebutton;
bool mouse_rightbutton;

static int32_t mouse_x;
static int32_t mouse_y;
static int32_t mouse_last_x;
static int32_t mouse_last_y;
uint32_t *back_data;

void ps2mouse_init(uint32_t starting_x, uint32_t starting_y, uint32_t border_width, uint32_t border_height);
void ps2mouse_draw();
void ps2mouse_update();
void ps2mouse_hide();
void ps2mouse_show();
void ps2mouse_add_handler(void (*mouse_handler)(uint32_t x, uint32_t y, bool left, bool right, bool middle));
void ps2mouse_remove_handler();