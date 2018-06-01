
#include <sysprint.h>
#include <stdbool.h>
#include <stdint.h>
//#include <stdarg.h>
#include <string.h>

#include <system.h>
#include <serial.h>
#include <heap.h>
#include <gdt.h>
#include <io.h>

bool scroll = true;
uint8_t x;
uint8_t y;
uint8_t color;
uint8_t init_color;
uint16_t* buf;

void scrollup();
void set_cursor_pos(uint8_t x, uint8_t y);

void init_printer(int fg, int bg)
{
	//x = 0;
	//y = 0;
	init_color = color = fg | bg << 4;
	buf = (uint16_t *)0x000B8000;
	memset(buf, color << 8 | 0x20, 80 * 25);

	//tss_t* mytss = (tss_t*)(_gdt_get_ptr()[5].base2 << 24 | _gdt_get_ptr()[5].base1 << 16 | _gdt_get_ptr()[5].base0);
	////_sys_v86mode(mytss->ss, mytss->esp, mytss->cs, mytss->eip);

	//// Disable blinking text
	//inb(0x3DA);
	////uint8_t addrdatareg = inb(0x3C0);
	//outb(0x3C0, 0x10);
	////uint8_t regvalue = inb(0x3C1) & 0xF7;
	////outb(0x3C0, regvalue);
	////outb(0x3C0, addrdatareg);
	////inb(0x3DA);
}

uint8_t get_x_pos() {
	return x;
}

uint8_t get_y_pos() {
	return y;
}

void lock_scroll() {
	scroll = false;
}

void unlock_scroll() {
	scroll = true;
}

void set_position(uint8_t x_pos, uint8_t y_pos)
{
	x = x_pos;
	y = y_pos;
}

void set_cursor_pos(uint8_t x, uint8_t y)
{
	uint16_t pos = y * 80 + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, pos & 0xFF);

	outb(0x3D4, 0x0E);
	outb(0x3D5, (pos >> 8) & 0xFF);
}

void set_cursor_visible(bool visible)
{
	if (visible) {
		outb(0x3D4, 0x0A);
		outb(0x3D5, (inb(0x3D5) & 0xC0) | 0);
		
		outb(0x3D4, 0x0B);
		outb(0x3D5, (inb(0x3D5) & 0xE0) | 6);
	} else {
		outb(0x3D4, 0x0A);
		outb(0x3D5, 0x20);
	}
}

void scrollup()
{
	if (scroll) {
		for (int i = 0; i < 25; i++)
		{
			uint16_t* dest = 0xB7F60 + (i * 160);
			uint16_t* src = 0xB8000 + (i * 160);
			for (int i = 0; i < 80; i++)
			{
				dest[i] = src[i];
			}
		}
	
		for (int i = 0; i < 80; i++)
			buf[24 * 80 + i] = color << 8 | ' ';
	}
}

void printstrl(char *value, size_t length)
{
	for (int i = 0; i < length; i++)
		printchr(value[i]);
}

// Print regular char *
void printstr(char *value)
{
	for (int i = 0; i < lengthof(value); i++) {
		printchr(value[i]);
	}
}

void printchr(char value)
{
	if (value == '\n')
	{
		y++;
		x = 0;
		set_cursor_pos(x, y);
		return;
	}
	
	if (y == 25) 
	{
		scrollup();
		y--;
	}

	buf[(y * 80) + x++] = (color << 8 | value);
	
	if (x == 80 && y < 25) {
		x = 0;
		y++;
	}
	set_cursor_pos(x, y);
}

void printnum(int32_t value)
{
	printstr(int2str(value));
}

void printhex(uint32_t value, size_t length)
{
	printstr("0x");
	printstr(hex2str(value, length));
}

void sprintstr(int port_num, char *value)
{
	for (int i = 0; i < lengthof(value); i++) {
		sprintchr(port_num, value[i]);
	}
}

void sprintchr(int port_num, char value)
{
	serial_select(port_num);
	serial_write(value);
}

void sprintnum(int port_num, int32_t value)
{
	sprintstr(port_num, int2str(value));
}

void sprinthex(int port_num, uint32_t value, size_t length)
{
	sprintstr(port_num, "0x");
	sprintstr(port_num, hex2str(value, length));
}

// Print formatted char *
// [%c<color>]
// Set foreground color
// Dark colors should have small letter
// Normal colors should have capital letter
// Black color is 0 due to Blue and Dark blue
// Brown color is 1 due to Blue and Dark blue
// Gray and dark gray has the character 'D/d'
// (R = Red, r = Dark red, B = Blue, b = Dark blue, 0 = black)
// [---------]
// [%b<color>]
// Set background color
// Same as [%c<color>]
void printstrf(char *value)
{
	//uint32_t uint_args[16];
	//for (int i = 0; i < 16; i++)
	//	uint_args[i] = va_arg(args, uint32_t);

	bool no_restore;
	uint8_t hexlen;
	for (int i = 0; i < lengthof(value); i++)
	{
		switch (value[i])
		{
		case '%': // Prefix
			switch (value[i + 1])
			{
			case 'b': // Background switch
				switch (value[i + 2])
				{
				case '0':
					color = 0x0 << 4 | (color & 0xF);
					i += 2;
					break;
				case 'b':
					color = 0x1 << 4 | (color & 0xF);
					i += 2;
					break;
				case 'g':
					color = 0x2 << 4 | (color & 0xF);
					i += 2;
					break;
				case 'c':
					color = 0x3 << 4 | (color & 0xF);
					i += 2;
					break;
				case 'r':
					color = 0x4 << 4 | (color & 0xF);
					i += 2;
					break;
				case 'm':
					color = 0x5 << 4 | (color & 0xF);
					i += 2;
					break;
				case '1':
					color = 0x6 << 4 | (color & 0xF);
					i += 2;
					break;
				case 'D':
					color = 0x7 << 4 | (color & 0xF);
					i += 2;
					break;
				case 'd':
					color = 0x8 << 4 | (color & 0xF);
					i += 2;
					break;
				case 'B':
					color = 0x9 << 4 | (color & 0xF);
					i += 2;
					break;
				case 'G':
					color = 0xA << 4 | (color & 0xF);
					i += 2;
					break;
				case 'C':
					color = 0xB << 4 | (color & 0xF);
					i += 2;
					break;
				case 'R':
					color = 0xC << 4 | (color & 0xF);
					i += 2;
					break;
				case 'M':
					color = 0xD << 4 | (color & 0xF);
					i += 2;
					break;
				case 'Y':
					color = 0xE << 4 | (color & 0xF);
					i += 2;
					break;
				case 'W':
					color = 0xF << 4 | (color & 0xF);
					i += 2;
					break;
				}
				break;
			case 'c': // Color (Foreground) switch
				switch (value[i + 2])
				{
				case '0':
					color = (color >> 4) << 4 | 0x0;
					i += 2;
					break;
				case 'b':
					color = (color >> 4) << 4 | 0x1;
					i += 2;
					break;
				case 'g':
					color = (color >> 4) << 4 | 0x2;
					i += 2;
					break;
				case 'c':
					color = (color >> 4) << 4 | 0x3;
					i += 2;
					break;
				case 'r':
					color = (color >> 4) << 4 | 0x4;
					i += 2;
					break;
				case 'm':
					color = (color >> 4) << 4 | 0x5;
					i += 2;
					break;
				case '1':
					color = (color >> 4) << 4 | 0x6;
					i += 2;
					break;
				case 'D':
					color = (color >> 4) << 4 | 0x7;
					i += 2;
					break;
				case 'd':
					color = (color >> 4) << 4 | 0x8;
					i += 2;
					break;
				case 'B':
					color = (color >> 4) << 4 | 0x9;
					i += 2;
					break;
				case 'G':
					color = (color >> 4) << 4 | 0xA;
					i += 2;
					break;
				case 'C':
					color = (color >> 4) << 4 | 0xB;
					i += 2;
					break;
				case 'R':
					color = (color >> 4) << 4 | 0xC;
					i += 2;
					break;
				case 'M':
					color = (color >> 4) << 4 | 0xD;
					i += 2;
					break;
				case 'Y':
					color = (color >> 4) << 4 | 0xE;
					i += 2;
					break;
				case 'W':
					color = (color >> 4) << 4 | 0xF;
					i += 2;
					break;
				}
				break;
			case 'h':
				switch (value[i + 2])
				{
				case '0':
					//for (hexlen = 0; hexlen; hexlen++, uint_args[0] >>= 4);
					//printhex(uint_args[0], hexlen);
					i += 2;
					break;
				case '1':
					switch (value[i + 3])
					{
					case '0':
						//for (hexlen = 0; hexlen; hexlen++, uint_args[10] >>= 4);
						//printhex(uint_args[10], hexlen);
						i += 3;
						break;
					case '1':
						//for (hexlen = 0; hexlen; hexlen++, uint_args[11] >>= 4);
						//printhex(uint_args[11], hexlen);
						i += 3;
						break;
					case '2':
						//for (hexlen = 0; hexlen; hexlen++, uint_args[12] >>= 4);
						//printhex(uint_args[12], hexlen);
						i += 3;
						break;
					case '3':
						//for (hexlen = 0; hexlen; hexlen++, uint_args[13] >>= 4);
						//printhex(uint_args[13], hexlen);
						i += 3;
						break;
					case '4':
						//for (hexlen = 0; hexlen; hexlen++, uint_args[14] >>= 4);
						//printhex(uint_args[14], hexlen);
						i += 3;
						break;
					case '5':
						//for (hexlen = 0; hexlen; hexlen++, uint_args[15] >>= 4);
						//printhex(uint_args[15], hexlen);
						i += 3;
						break;
					default:
						//for (hexlen = 0; hexlen; hexlen++, uint_args[1] >>= 4);
						//printhex(uint_args[1], hexlen);
						i += 2;
						break;
					}
					break;
				case '2':
					//for (hexlen = 0; hexlen; hexlen++, uint_args[2] >>= 4);
					//printhex(uint_args[2], hexlen);
					i += 2;
					break;
				case '3':
					//for (hexlen = 0; hexlen; hexlen++, uint_args[3] >>= 4);
					//printhex(uint_args[3], hexlen);
					i += 2;
					break;
				case '4':
					//for (hexlen = 0; hexlen; hexlen++, uint_args[4] >>= 4);
					//printhex(uint_args[4], hexlen);
					i += 2;
					break;
				case '5':
					//for (hexlen = 0; hexlen; hexlen++, uint_args[5] >>= 4);
					//printhex(uint_args[5], hexlen);
					i += 2;
					break;
				case '6':
					//for (hexlen = 0; hexlen; hexlen++, uint_args[6] >>= 4);
					//printhex(uint_args[6], hexlen);
					i += 2;
					break;
				case '7':
					//for (hexlen = 0; hexlen; hexlen++, uint_args[7] >>= 4);
					//printhex(uint_args[7], hexlen);
					i += 2;
					break;
				case '8':
					//for (hexlen = 0; hexlen; hexlen++, uint_args[8] >>= 4);
					//printhex(uint_args[8], hexlen);
					i += 2;
					break;
				case '9':
					//for (hexlen = 0; hexlen; hexlen++, uint_args[9] >>= 4);
					//printhex(uint_args[9], hexlen);
					i += 2;
					break;
				}
				break;
			case 'n':
				i++;
				no_restore = true;
				break;
			case 'i':
				switch (value[i + 2])
				{
				case '0':
					//printnum(uint_args[0]);
					i += 2;
					break;
				case '1':
					switch (value[i + 3])
					{
					case '0':
						//printnum(uint_args[10]);
						i += 3;
						break;
					case '1':
						//printnum(uint_args[11]);
						i += 3;
						break;
					case '2':
						//printnum(uint_args[12]);
						i += 3;
						break;
					case '3':
						//printnum(uint_args[13]);
						i += 3;
						break;
					case '4':
						//printnum(uint_args[14]);
						i += 3;
						break;
					case '5':
						//printnum(uint_args[15]);
						i += 3;
						break;
					default:
						//printnum(uint_args[1]);
						i += 2;
						break;
					}
					break;
				case '2':
					//printnum(uint_args[2]);
					i += 2;
					break;
				case '3':
					//printnum(uint_args[3]);
					i += 2;
					break;
				case '4':
					//printnum(uint_args[4]);
					i += 2;
					break;
				case '5':
					//printnum(uint_args[5]);
					i += 2;
					break;
				case '6':
					//printnum(uint_args[6]);
					i += 2;
					break;
				case '7':
					//printnum(uint_args[7]);
					i += 2;
					break;
				case '8':
					//printnum(uint_args[8]);
					i += 2;
					break;
				case '9':
					//printnum(uint_args[9]);
					i += 2;
					break;
				}
				break;
			}
			break;
		default:
			printchr(value[i]);
		}
	}
	if (!no_restore)
	color = init_color;
}

void printer_clr()
{
	memset(buf, color << 8 | 0x20, 80 * 25);
	x = 0;
	y = 0;
}

void printinf(char *info)
{
	if (x > 0) printchr('\n');
	printstrf("%cB[Info] %n");
	printstrf(info);
	printstrf("\n");
}

void printwrn(char *warning)
{
	if (x > 0) printchr('\n');
	printstrf("%cY[Warning] %n");
	printstrf(warning);
	printstrf("\n");
}

void printerr(char *error, errorcode_t errorcode, bool panic)
{
	if (x > 0) printchr('\n');
	if (panic)
	{
		printstrf("%cr[Panic] %n");
		printstrf(error);
		printchr('\n');

		printstrf("%cR%n");
		printstr("Error code: ");
		printstr(hex2str((uint32_t)errorcode, 8));
		printchr('\n');
		printstrf("Restart your PC manually!");
		//printstrf("Press any key to restart!");
		//power_restart();
	}
	else
	{
		printstrf("%cR[Error] %n");
		printstrf(error);
		printstrf("\n");
	}
}