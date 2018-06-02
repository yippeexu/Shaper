
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <ps2_mouse.h>
#include <vesa.h>
#include <irq.h>
#include <io.h>

bool acknowledged = true;
static bool overdrawn;
static bool guimouse;
static bool hide_mouse;
static uint32_t border_w;
static uint32_t border_h;
uint8_t *mouse_imgfile;
void (*ps2_mousehandler)(uint32_t x, uint32_t y, bool left, bool right, bool middle);

void ps2mouse_handler(x86_regs *context);
void mouse_wait(uint8_t type);

void ps2mouse_init(uint32_t starting_x, uint32_t starting_y, uint32_t border_width, uint32_t border_height)
{
    //printer_clr();

    //lock_scroll();
    _irq_unmask(0x0C);
    set_cursor_visible(false);

    printstr("Initializing Mouse\n");

    overdrawn = false;
    hide_mouse = false;
    mouse_x = starting_x;
    mouse_y = starting_y;
    border_w = border_width;
    border_h = border_height;
    back_data = (uint32_t *)0xEF0000;

    mouse_wait(1);
    outb(0x64, 0xA8);

    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    uint8_t status = (inb(0x60) | (1 << 1));

    printstr("Power-up status: ");
    for (int i = 0; i < 8; i++) {
        printnum(status >> i & 1);
    }
    printchr('\n');

    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status);
    
    mouse_wait(1);
    outb(0x64, 0xD4);
    mouse_wait(1);
    outb(0x60, 0xF6);
    
    mouse_wait(1);
    outb(0x64, 0xD4);
    mouse_wait(1);
    outb(0x60, 0xF4);

    mouse_wait(0);
    inb(0x60);

    //mouse_wait(1);
    //outb(0x64, 0xD4);
    //mouse_wait(1);
    //outb(0x60, 0xF2);
//
    //mouse_wait(0);
    //printhex(inb(0x60), 2);
    //
    //printchr('\n');

    _irq_add_handler(0x0C, ps2mouse_handler);
}

void mouse_wait(uint8_t type)
{
    switch (type)
    {
    case 0:
        for (int i = 0; i < 1000000; i++)
            if ((inb(0x64) & 1) == 1)
                return;

        return;
    case 1:
        for (int i = 0; i < 1000000; i++)
            if ((inb(0x64) & 2) == 2)
                return;
                
        return;
    }
}

void ps2mouse_handler(x86_regs *context)
{
    sprintstr(0, "IRQ 12\n");

    static uint8_t irq_cycle = 0;
    static int8_t mouse_data[3];
    mouse_data[irq_cycle++] = inb(0x60);

    if (irq_cycle == 3)
    {
        irq_cycle = 0;
        
        if (acknowledged) {
            acknowledged = false;

            if ((mouse_data[0] & 0x80) || (mouse_data[0] & 0x40))
                return;

            if (!(mouse_data[0] & 0x10))
                mouse_data[1] |= 0xFFFFFF00; //-mouse_data[1];
                
            if (!(mouse_data[0] & 0x20))
                mouse_data[2] |= 0xFFFFFF00;
                
            mouse_leftbutton = mouse_data[0] & 1;
            mouse_rightbutton = mouse_data[0] & 2;
            mouse_middlebutton = mouse_data[0] & 4;

            mouse_last_x = mouse_x;
            mouse_last_y = mouse_y;
            mouse_x += mouse_data[1];
            mouse_y += -mouse_data[2];

            ps2mouse_update();
            if (ps2_mousehandler)
                ps2_mousehandler(mouse_x, mouse_y, mouse_leftbutton, mouse_rightbutton, mouse_middlebutton);
        }
    }
}

void ps2mouse_draw()
{
    guimouse = true;
    //vga_draw_mouse(mouse_x, mouse_y);
}

void ps2mouse_update()
{
    static int w = 0;
    static int h = 0;

    if (!hide_mouse) {
        //_irq_remove_handler(0x0C);

        if (mouse_x > border_w-1)
            mouse_x = border_w-2;
        if (mouse_last_x < border_w/2.5 && mouse_x > border_w-border_w/2.5)
            mouse_x = 1;
        if (mouse_y > border_h-1)
            mouse_y = border_h-2;
        if (mouse_last_y < border_h/2.5 && mouse_y > border_h-border_h/2.5)
            mouse_y = 1;

        if (guimouse) {
            //sgs_set_mouse(mouse_x, mouse_y);
        } else {
            set_position(mouse_last_x, mouse_last_y);
            printchr(0);
            set_position(mouse_x, mouse_y);
            printchr(0x18);
            set_position(mouse_x, mouse_y);
        }
        //_irq_add_handler(0x0C, ps2mouse_handler);
    }

    acknowledged = true;
}

void ps2mouse_hide()
{
    hide_mouse = true;
    if (guimouse) {
        //sgs_set_mouse(mouse_x, mouse_y);
    } else {
        set_position(mouse_x, mouse_y);
        printchr(0);
        set_position(mouse_x, mouse_y);
    }
}

void ps2mouse_show()
{
    hide_mouse = false;
}

void ps2mouse_add_handler(void (*mouse_handler)(uint32_t x, uint32_t y, bool left, bool right, bool middle))
{
    ps2_mousehandler = mouse_handler;
}

void ps2mouse_remove_handler()
{
    ps2_mousehandler = 0;
}