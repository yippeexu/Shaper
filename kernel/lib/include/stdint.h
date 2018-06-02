
#include <stddef.h>

#ifndef $_CSTDINT
#define $_CSTDINT

// Signed defines
#define INT8_MIN  -0x80
#define INT16_MIN -0x8000
#define INT32_MIN -0x80000000
#define INT64_MIN -0x8000000000000000

#define INT_FAST8_MIN  INT32_MIN
#define INT_FAST16_MIN INT32_MIN
#define INT_FAST32_MIN INT32_MIN
#define INT_FAST64_MIN INT32_MIN

#define INT_LEAST8_MIN  INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN

#define INTPTR_MIN INT16_MIN
#define INTMAX_MIN INT32_MIN

#define INT8_MAX  0x7F
#define INT16_MAX 0x7FFF
#define INT32_MAX 0x7FFFFFFF
#define INT64_MAX 0x7FFFFFFFFFFFFFFF

#define INT_FAST8_MAX  INT32_MAX
#define INT_FAST16_MAX INT32_MAX
#define INT_FAST32_MAX INT32_MAX
#define INT_FAST64_MAX INT32_MAX

#define INT_LEAST8_MAX  INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX

#define INTPTR_MAX INT16_MAX
#define INTMAX_MAX INT32_MAX

// Unsigned defines
#define UINT8_MAX  0xFF
#define UINT16_MAX 0xFFFF
#define UINT32_MAX 0xFFFFFFFF
#define UINT64_MAX 0xFFFFFFFFFFFFFFFF

#define UINT_FAST8_MAX  UINT32_MAX
#define UINT_FAST16_MAX UINT32_MAX
#define UINT_FAST32_MAX UINT32_MAX
#define UINT_FAST64_MAX UINT32_MAX

#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define UINTPTR_MAX UINT16_MAX
#define UINTMAX_MAX UINT32_MAX

// Other defines
#define SIZE_MAX UINT32_MAX
#define PTRDIFF_MIN INTPTR_MIN
#define PTRDIFF_MAX INTPTR_MAX
#define SIG_ATOMIC_MIN INT32_MIN
#define SIG_ATOMIC_MAX INT32_MAX
#define WCHAR_MIN 0x0000
#define WCHAR_MAX 0xFFFF
#define WINT_MIN  0x0000
#define WINT_MAX  0xFFFF

// Signed macros
#define INT8_C(x)  ((int8_t)(x))
#define INT16_C(x) ((int16_t)(x))
#define INT32_C(x) ((int32_t)(x))
#define INT64_C(x) ((int64_t)(x))

#define INTMAX_C(x) ((intmax_t)(x))

// Unsigned macros
#define UINT8_C(x)  ((uint8_t)(x))
#define UINT16_C(x) ((uint16_t)(x))
#define UINT32_C(x) ((uint32_t)(x))
#define UINT64_C(x) ((uint64_t)(x))

#define UINTMAX_C(x) ((uintmax_t)(x))

// Signed types

typedef char      int8_t;
typedef short     int16_t;
typedef char      int24_t[3];
typedef long      int32_t;
typedef long long int64_t;

typedef int32_t int_fast8_t;
typedef int32_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int32_t int_fast64_t;

typedef int* intptr_t;
typedef long intmax_t;

// Unsigned types
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned char      uint24_t[3];
typedef unsigned long      uint32_t;
typedef unsigned long long uint64_t;

typedef uint32_t uint_fast8_t;
typedef uint32_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint32_t uint_fast64_t;

typedef unsigned long uintmax_t;
typedef unsigned int* uintptr_t;

#endif