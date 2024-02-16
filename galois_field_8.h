#ifndef _GALOIS_FIELD_8_H_
#define _GALOIS_FIELD_8_H_

// Represents field operations in GF(2^8)

#include <stdint.h>

uint8_t gf8_add(uint8_t a, uint8_t b);

uint8_t gf8_sub(uint8_t a, uint8_t b);

#endif