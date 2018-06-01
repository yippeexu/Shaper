
#include <stdbool.h>
#include <stdint.h>

#ifndef $_CSTRING
#define $_CSTRING

#define lengthof(str) strlen(str)
#define countof(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef unsigned int size_t;

void  *memcpy(void *dest, void* src, size_t count);
void  *kmemcpy(void *dest, void* src, size_t count);
int    memcmp(void *ptr1, void *ptr2, size_t count);
void  *memset(void *dest, int value, size_t count);
void  *kmemset(void *dest, int value, size_t count);
void  *memclr(void *dest, size_t count);
void  *memchr(void *dest, char chr, size_t count);
void  *memmove(void *dest, void *src, size_t count);
char *strcat(char *dest, char *src);
char *strchr(char *str, char chr);
int    strcmp(char *str1, char *str2);
int    strcoll(char *str1, char *str2);
char *strcpy(char *dest, char *src);
size_t strcspn(char *str1, char *str2);
char *strerror(int errornum);
size_t strlen(char *str);
char *strncat(char *dest, char *src, size_t num);
int    strncmp(char *str1, char *str2, size_t num);
char *strncpy(char *dest, char *src, size_t num);
char *strpbrk(char *str1, char *str2);
char *strrchr(char *str, char chr);
size_t strspn(char *str1, char *str2);
char *strstr(char *str1, char *str2);
char *strtok(char *str, char *delimiters);
char *int2str(int32_t integer);
int32_t str2int(char *integer_str);
char *hex2str(uint32_t integer, size_t length);

#endif