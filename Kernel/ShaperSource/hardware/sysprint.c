#include <sysprint.h>
#include <io.h>
#include <stdbool.h>
#include <stdint.h>
#include <system.h>
#include <tss.h>
#include <gdt.h>

uint8_t x;
uint8_t y;
uint8_t color;
uint8_t init_color;
uint16_t* buf;

void init_printer(int fg, int bg)
{
	//x = 0;
	//y = 0;
	init_color = color = fg | bg << 4;
	buf = (uint16_t *)0x000B8000;
	memset(buf, color << 8 | 0x20, 80 * 25);

	//tss_t* mytss = (tss_t*)(_gdt_get_ptr()[5].base2 << 24 | _gdt_get_ptr()[5].base1 << 16 | _gdt_get_ptr()[5].base0);
	//_sys_v86mode(mytss->ss, mytss->esp, mytss->cs, mytss->eip);

	//// Disable blinking text
	//inb(0x3DA);
	////uint8_t addrdatareg = inb(0x3C0);
	//outb(0x3C0, 0x10);
	////uint8_t regvalue = inb(0x3C1) & 0xF7;
	////outb(0x3C0, regvalue);
	////outb(0x3C0, addrdatareg);
	////inb(0x3DA);
}

// Print regular string
void printstr(string value)
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
		return;
	}

	if (x == 80) {
		x = 0;
		y++;
	}
	buf[(y * 80) + x] = (color << 8 | value);
	x++;
}

void printnum(uint64_t value)
{
	printstr(int2str(value));
}

//void printhex(uint64_t value, size_t length)
//{
//}

// Print formatted string
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
void printstrf(string value)
{
	bool no_restore;
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
			case 'n':
				i++;
				no_restore = true;
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

void printinf(string info)
{
	if (x > 0) printchr('\n');
	printstrf("%cB[Info] %n");
	printstr(info);
	printstrf("\n");
}

void printwrn(string warning)
{
	if (x > 0) printchr('\n');
	printstrf("%cY[Warning] %n");
	printstr(warning);
	printstrf("\n");
}

void printerr(string error, errorcode_t errorcode, bool panic)
{
	if (x > 0) printchr('\n');
	if (panic)
	{
		printstrf("%cr[Panic] %n");
		printstr(error);
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
		printstr(error);
		printstrf("\n");
	}
}