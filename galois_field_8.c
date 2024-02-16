#include "galois_field_8.h"

uint8_t gf8_add(uint8_t a, uint8_t b)
{
    return a ^ b;
}

uint8_t gf8_sub(uint8_t a, uint8_t b)
{
    return a ^ b;
}