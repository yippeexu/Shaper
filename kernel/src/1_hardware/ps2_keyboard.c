
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <ps2_keyboard.h>
#include <blockdevice.h>
#include <key_layout.h>
#include <irq.h>
#include <pic.h>
#include <io.h>

void (*keypressed_handler)(uint8_t scancode, bool released);
void key_handler();

bool keyboard_handled;
uint8_t keyboard_scancode;
char *current_layout;

char get_keyboardkey(int scancode)
{
    if (scancode > 0x53) return 0;
    char key = current_layout[scancode*2+shiftpressed];
    return key;
}

void keyboard_init(void)
{
    current_layout = layout_en_US;
    _irq_unmask(0x01);
    _irq_add_handler(0x01, key_handler);
}

char keyboard_get_char()
{
    keyboard_handled = false;
    while (!keyboard_handled) ;
    return get_keyboardkey(keyboard_scancode);
}

void key_handler()
{
    keyboard_scancode = inb(0x60);
    bool released = keyboard_scancode & 0x80;

    if (released) {
        keyboard_scancode ^= 0x80;
    }

    switch (keyboard_scancode + released * 0x100)
    {
        case KEY_LCONTROL:
        case KEY_RCONTROL:
            ctrlpressed = true;
            return;
        case KEY_LSHIFT:
        case KEY_RSHIFT:
            shiftpressed = true;
            return;
        case KEY_LMENU:
        case KEY_RMENU:
            altpressed = true;
            return;
        case KEY_LCONTROL+0x100:
        case KEY_RCONTROL+0x100:
            ctrlpressed = false;
            return;
        case KEY_LSHIFT+0x100:
        case KEY_RSHIFT+0x100:
            shiftpressed = false;
            return;
        case KEY_LMENU+0x100:
        case KEY_RMENU+0x100:
            altpressed = false;
            return;
    }

    if (shiftpressed && altpressed) {
        shiftpressed = false;
        altpressed = false;
        //change_layout();
    }
    
    keyboard_handled = true;

    if (keypressed_handler)
        keypressed_handler(keyboard_scancode, released);
}

void keyboard_add_handler(void (*handler)(uint8_t scancode, bool released))
{
    keypressed_handler = handler;
}

void keyboard_remove_handler()
{
    keypressed_handler = 0;
}