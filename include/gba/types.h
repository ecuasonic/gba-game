#ifndef TYPES_H
#define TYPES_H

#include <stddef.h> // For NULL

#define ALIGN(n) __attribute__((aligned(n)))
#define NORETURN __attribute__((noreturn))

typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef signed int   s32;
typedef signed short s16;
typedef signed char  s8;

typedef volatile unsigned int   vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile signed int   vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

#define INLINE inline static

#endif // TYPES_H
