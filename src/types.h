#ifndef TYPES_H
#define TYPES_H

/* unsigned types */
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

/* signed types */
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

/* Address and Size types */
typedef unsigned long long uintptr_t;
typedef long long intptr_t;
typedef unsigned long long size_t;

/* Useful constants */
#define NULL ((void *)0)

/* Boolean support */
typedef enum { false = 0, true = 1 } bool;

#endif
