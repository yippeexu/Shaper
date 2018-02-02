
#ifndef $_STDDEF_H
#define $_STDDEF_H

#define NULL ((void*)0)

#define offsetof(x, y) ((int)(&(((x*)(0))->y)))

typedef int* ptrdiff_t;
typedef unsigned int size_t;
typedef void* nullptr_t;

#endif