
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define printbochs(msg) outsb(0xE9, msg, lengthof(msg))

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

typedef enum tag_errorcode errorcode_t;
typedef enum tag_errorcode
{
	ERRCODE_NONE
} errorcode_t;

void set_cursor_visible(bool visible);
void init_printer(int fg, int bg);
void lock_scroll();
void unlock_scroll();
uint8_t get_x_pos();
uint8_t get_y_pos();
void set_position(uint8_t x, uint8_t y);
void printstrl(char *value, size_t length);
void printstrf(char *value);
void printstr(char *value);
void printchr(char value);
void printnum(uint32_t value);
void printhex(uint32_t value, size_t length);
void sprintstr(int port_num, char *value);
void sprintchr(int port_num, char value);
void sprintnum(int port_num, uint32_t value);
void sprinthex(int port_num, uint32_t value, size_t length);
char *scan();
void printinf(char *info);
void printwrn(char *warning);
void printerr(char *error, errorcode_t errorcode, bool panic);
void printer_clr();
#endif