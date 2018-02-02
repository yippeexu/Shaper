
#include <string.h>
#include <stdbool.h>

#ifndef $_SYSPRINT_H
#define $_SYSPRINT_H

enum tag_cga16_colors
{
	CGA16_BLACK,
	CGA16_BLUE,
	CGA16_GREEN,
	CGA16_CYAN,
	CGA16_RED,
	CGA16_MAGENTA,
	CGA16_BROWN,
	CGA16_LIGHTGREY,
	CGA16_DARKGREY,
	CGA16_LIGHTBLUE,
	CGA16_LIGHTGREEN,
	CGA16_LIGHTCYAN,
	CGA16_LIGHTRED,
	CGA16_LIGHTMAGENTA,
	CGA16_YELLOW,
	CGA16_WHITE
};

typedef enum tag_errorcode
{
	ERRCODE_NONE
} errorcode_t;

void init_printer(int fg, int bg);
void printstrf(string value);
void printstr(string value);
void printchr(char value);
void printnum(uint64_t value);
//void printhex(uint64_t value, size_t length);
void printinf(string info);
void printwrn(string warning);
void printerr(string error, errorcode_t errorcode, bool panic);
void printer_clr();

#endif