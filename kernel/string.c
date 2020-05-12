
#include "types.h"


void *memset(void *s, uint8_t c, uint32_t n) {
    uint32_t x = 0;
    uint8_t *a = (uint8_t *)s; 
    while (x < n) {
	a[x++] = c; 
    }
    return s;
}

