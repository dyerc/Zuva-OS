#ifndef UTILS_H
#define UTILS_H

#include <types.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')

void memset(void* dest, uint8_t val, uint32_t len);
void memcpy(void* dest, void* src, uint32_t len);
void* memmove( void* dest, const void* src, uint32_t n);


int8_t ctoi(char c);
void itoa(int32_t n, char* s);
void i2hex(uint32_t val, char* dest, int32_t len);
float atof(const char* s);
void ftoa(float f, char* buffer);

#endif
