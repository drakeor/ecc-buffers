#include "galois_field_8.h"

// Credits:
// Help from https://en.wikiversity.org/wiki/Reed%E2%80%93Solomon_codes_for_coders for practical implementations,
//      especially for the optimized parts such as polynomial evaluations and division parts which were faster than mine.
// Other help from https://web.eecs.utk.edu/~jplank/plank/papers/CS-07-593/ especially for verifying my code.
// Lastly, theoretical help from https://downloads.bbc.co.uk/rd/pubs/whp/whp-pdf-files/WHP031.pdf for debugging

// Some constants
#define GF8_PRIMITIVE_POLYNOMIAL 0x11D
#define GF8_PRIMITIVE_ELEMENT 0x03
#define GF8_EXP_TABLE_SIZE 256
#define GF8_LOG_TABLE_SIZE 256

// Lookup tables for multiplications in GF(2^8)
uint8_t gf8_exp[GF8_EXP_TABLE_SIZE] = {0};
uint8_t gf8_log[GF8_LOG_TABLE_SIZE] = {0};

int gf8_init()
{
    // Initialize the lookup tables for multiplications in GF(2^8)
    uint8_t x = 1;
    for(uint8_t i = 0; i < 0xFF; i++)
    {
        gf8_exp[i] = x;
        gf8_log[x] = i;
        x = gf8_mul_nolut(x, 2);
    }
}

uint8_t gf8_add(uint8_t a, uint8_t b)
{
    // Adding two numbers in GF(2^8) is the same as XOR
    return a ^ b;
}

uint8_t gf8_sub(uint8_t a, uint8_t b)
{
    // Note that subtraction is the same as addition in GF(2^8)
    return a ^ b; 
}

// Uses the https://www.cut-the-knot.org/Curriculum/Algebra/PeasantMultiplication.shtml
uint8_t gf8_mul_nolut(uint8_t a, uint8_t b)
{
    // We allocate a larger buffer to work with as a might overflow 8 bits.
    uint16_t buffer = 0;
    uint8_t r = 0;

    // While b is above 0
    while(b > 0)
    {
        // If is an odd number, add a to the buffer
        if(b & 1)
        {
            r = gf8_add(buffer, r);
        }
        // Multiply b by 2
        buffer = buffer << 1; 
        // Divide a by 2
        b = b >> 1; 
        // Do modular reduction if a is above 255 using the primative polynomial for GF(2^8)
        if(buffer > 0xFF)
        {
            buffer = gf8_add(buffer, GF8_PRIMITIVE_POLYNOMIAL);
        }
    }
    return r;
}

uint8_t gf8_mul(uint8_t a, uint8_t b)
{
    // Multiply two numbers in GF(2^8) using the lookup tables
    return gf8_exp[(gf8_log[a] + gf8_log[b]) % 0xFF];
}

uint8_t gf8_div(uint8_t a, uint8_t b)
{
    // Divide two numbers in GF(2^8) using the lookup tables
    if(a == 0 || b == 0)
    {
        return 0;
    }
    return gf8_exp[(gf8_log[a] + 0xFF - gf8_log[b]) % 0xFF];
}

uint8_t gf8_pow(uint8_t a, uint8_t pow)
{
    return gf8_exp[(gf8_log[a] * pow) % 0xFF];
}

uint8_t gf8_inv(uint8_t a)
{
    // Apparently this can be shortcutted to gf8_div(1,a)
    return gf8_exp[0xFF - gf8_log[a]];
}

int gf8_poly_scale(uint8_t* buffer, uint8_t* p, uint8_t scale, uint8_t p_len)
{
    for(int i = 0; i < p_len; i++)
    {
        buffer[i] = gf8_mul(p[i], scale);
    }
}

int gf8_poly_add(uint8_t* buffer, uint8_t* p, uint8_t* q, uint8_t p_len, uint8_t q_len)
{
    int max_len = p_len > q_len ? p_len : q_len;
    for(int i = 0; i < max_len; i++)
    {
        buffer[i + max_len - p_len] = p[i];
    }
    for(int i = 0; i < max_len; i++)
    {
        buffer[i + max_len - q_len] = gf8_add(buffer[i + max_len - q_len], q[i]);
    }
}

int gf8_poly_mul(uint8_t* buffer, uint8_t* p, uint8_t* q, uint8_t p_len, uint8_t q_len)
{
    for(int i = 0; i < p_len; i++)
    {
        for(int j = 0; j < q_len; j++)
        {
            buffer[i + j] = gf8_add(buffer[i + j], gf8_mul(p[i], q[j]));
        }
    }
}

int gf8_poly_div(uint8_t* buffer, uint8_t* p, uint8_t* q, uint8_t p_len, uint8_t q_len)
{
    
}

uint8_t gf8_poly_eval(uint8_t* p, uint8_t x, uint8_t p_len)
{
    int result = p[0];
    for(int i = 1; i < p_len; i++)
    {
        result = gf8_add(gf8_mul(result, x), p[i]);
    }
    return result;
}