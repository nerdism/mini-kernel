#ifndef PRINTF_H
#define PRINTF_H
#include "types.h"

void printf(const char *format, ...);

void print_str(char *s);

void print_int32(int32_t d);
void print_uint32(uint32_t d);
void print_hex(uint32_t d);

#endif

