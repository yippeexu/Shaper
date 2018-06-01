
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <io.h>

const uint16_t serial_ports[4] = { 0x3F8, 0x2F8, 0x3E8, 0x2E8 };

uint16_t current_port;

void serial_init(int port_num)
{
    if (port_num >= 4) return;
    current_port = serial_ports[port_num];
    printstr("(Serial-Port) Initializing COM");
    printnum(1+port_num);
    printchr('\n');
    outb(current_port + 1, 0x00);
    outb(current_port + 3, 0x80);
    outb(current_port + 0, 0x03);
    outb(current_port + 1, 0x00);
    outb(current_port + 3, 0x03);
    outb(current_port + 2, 0xC7);
    outb(current_port + 4, 0x0B);
    printstr("(Serial-Port) Done Initializing COM");
    printnum(1+port_num);
    printchr('\n');
    sprintchr(0, '\n');
    sprintstr(0, "-- Serial Port COM");
    sprintnum(0, 1+port_num);
    sprintchr(0, '\n');
}

void serial_select(int port_num)
{
    current_port = serial_ports[port_num];
}

char serial_read()
{
    while (!(inb(current_port + 5) & 1)) ;

    inb(current_port);
}

void serial_write(char value)
{
    while (!(inb(current_port + 5) & 0x20)) ;

    outb(current_port, value);
}