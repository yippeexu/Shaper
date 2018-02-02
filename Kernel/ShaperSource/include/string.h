
#include <stddef.h>
#include "stdint.h"

#ifndef $_STRING_H
#define $_STRING_H

#define lengthof(str) strlen(str)
#define countof(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef char *string;

//size_t will not work as expected
//typedef unsigned int size_t;

void  *memcpy(void *dest, void* src, size_t count);
int    memcmp(void *ptr1, void *ptr2, size_t count);
void  *memset(void *src, int value, size_t count);
void  *memclr(void *dest, size_t count);
void  *memchr(void *dest, char chr, size_t count);
void  *memmove(void *dest, void *src, size_t count);
string strcat(string dest, string src);
string strchr(string str, char chr);
int    strcmp(string str1, string str2);
int    strcoll(string str1, string str2);
string strcpy(string dest, string src);
size_t strcspn(string str1, string str2);
string strerror(int errornum);
size_t strlen(string str);
string strncat(string dest, string src, size_t num);
int    strncmp(string str1, string str2, size_t num);
string strncpy(string dest, string src, size_t num);
string strpbrk(string str1, string str2);
string strrchr(string str, char chr);
size_t strspn(string str1, string str2);
string strstr(string str1, string str2);
string strtok(string str, string delimiters);
string int2str(int integer);
string hex2str(uint32_t integer, size_t length);

#endif