#include "galois_field_8.h"

// Credits:
// Help from https://en.wikiversity.org/wiki/Reed%E2%80%93Solomon_codes_for_coders 
//      for practical implementations, especially for the optimized parts 
//      such as polynomial evaluations and division parts which were faster 
//      than mine.
// Other help from https://web.eecs.utk.edu/~jplank/plank/papers/CS-07-593/ 
//      especially for verifying my code.
// Lastly, theoretical help from https://downloads.bbc.co.uk/rd/pubs/whp/whp-pdf-files/WHP031.pdf 
//      for debugging

// For lazy initialization of the gf8 field library.
// By default, will initialize whenever it requires lookup tables 
// or other settings.
// If disabled, remember to call gf8_init() manually in your code.
#define GF8_LAZY_INIT 1
int gf8_initialized = 0;

// I selected this degree 8 primitive polynomial. 
// As long as it's consistent.. should be fine
#define GF8_PRIMITIVE_POLYNOMIAL 0x11D

// Size should be 2^8 + 1 to encompass all elements.
#define GF8_EXP_TABLE_SIZE (0xFF + 1)
#define GF8_LOG_TABLE_SIZE (0xFF + 1)

// Lookup tables for multiplications in GF(2^8)
uint8_t gf8_exp[GF8_EXP_TABLE_SIZE] = {0};
uint8_t gf8_log[GF8_LOG_TABLE_SIZE] = {0};

int gf8_init()
{
    // Initialize the lookup tables for multiplications in GF(2^8)
    uint8_t x = 1;
    for(uint8_t i = 0; i < 0xFF; i++) {
        gf8_exp[i] = x;
        gf8_log[x] = i;
        x = gf8_mul_nolut(x, 2);
    }
    gf8_initialized = 1;
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
    uint8_t r = 0;

    // While b is above 0
    while(b > 0) {
        // If b is an odd number, add a to the buffer
        if(b & 1) {
            r = r ^ a;
        }
        // Divide b by 2
        b >>= 1; 
        // If the left most bit is high, 
        //  divide by the primitive polynomial
        if(a & 0x80) {
            a = (a << 1) ^ GF8_PRIMITIVE_POLYNOMIAL;
        } else {
            a <<= 1;
        }
    }

    return r;
}

uint8_t gf8_mul(uint8_t a, uint8_t b)
{
    // Initialize GF8 if not already initialized
    if(GF8_LAZY_INIT && !gf8_initialized) {
        gf8_init();
    }

    // Trivial, return 0
    if(a == 0 || b == 0) {
        return 0;
    }

    // Multiply two numbers in GF(2^8) using the lookup tables
    return gf8_exp[(gf8_log[a] + gf8_log[b]) % 0xFF];
}

uint8_t gf8_div(uint8_t a, uint8_t b)
{
    // Initialize GF8 if not already initialized
    if(GF8_LAZY_INIT && !gf8_initialized) {
        gf8_init();
    }

    // Prevent divide by zero errors
    if(a == 0 || b == 0) {
        return 0;
    }
    // Divide two numbers in GF(2^8) using the lookup tables
    // Adding 0xFF allows us to not underflow
    // The mod 0xFF makes sure we stay within the bounds (and field)
    return gf8_exp[(gf8_log[a] + 0xFF - gf8_log[b]) % 0xFF];
}

uint8_t gf8_pow(uint8_t a, uint8_t pow)
{
    // Initialize GF8 if not already initialized
    if(GF8_LAZY_INIT && !gf8_initialized) {
        gf8_init();
    }

    return gf8_exp[(gf8_log[a] * pow) % 0xFF];
}

uint8_t gf8_inv(uint8_t a)
{
    // Initialize GF8 if not already initialized
    if(GF8_LAZY_INIT && !gf8_initialized) {
        gf8_init();
    }

    return gf8_div(1, a);
}

int gf8_poly_scale(uint8_t* buffer, uint8_t* p, 
    uint8_t scale, uint8_t p_len)
{
    // Simply multiply each coef by the scale
    for(int i = 0; i < p_len; i++) {
        buffer[i] = gf8_mul(p[i], scale);
    }
    return 0;
}

int gf8_poly_add(uint8_t* buffer, uint8_t* p, uint8_t* q, 
    uint8_t p_len, uint8_t q_len)
{
    // Pick the larger of the lengths
    int max_len = (p_len > q_len) ? p_len : q_len;
    
    // Zero out the buffer
    for(int i = 0; i < max_len; i++) {
        buffer[i] = 0;
    }

    // Fill the buffer with values from p first
    for(int i = 0; i < p_len; i++) {
        buffer[i + max_len - p_len] = p[i];
    }

    // Add in q next
    for(int i = 0; i < q_len; i++) {
        buffer[i + max_len - q_len] = 
            gf8_add(buffer[i + max_len - q_len], q[i]);
    }
    return 0;
}

int gf8_poly_mul(uint8_t* buffer, uint8_t* p, uint8_t* q, 
    uint8_t p_len, uint8_t q_len)
{
    // Zero out the buffer
    for(int i = 0; i < p_len + q_len - 1; i++) {
        buffer[i] = 0;
    }

    // This is your run of the mill polynomial multiplication
    for(int i = 0; i < p_len; i++) {
        for(int j = 0; j < q_len; j++) {
            buffer[i + j] ^= gf8_mul(p[i], q[j]);
        }
    }
    return 0;
}

int gf8_poly_div(uint8_t* buffer_quotient, uint8_t* buffer_remainder, 
    uint8_t* p, uint8_t* q, uint8_t p_len, uint8_t q_len)
{
    // Sanity checks, divisor (q) can't be larger than dividend (p)
    if(q_len >= p_len) {
        return -1;
    }
    // q_len and p_len can't be 0
    if(p_len == 0 || q_len == 0) {
        return -1;
    }

    // Copy the dividend (p) to quotient buffer
    for(int i = 0; i < p_len; i++) {
        buffer_quotient[i] = p[i];
    }

    // Zero out the remainder for now
    for(int i = 0; i < q_len; i++) {
        buffer_remainder[i] = 0;
    }

    // If p_len and q_len are the same size, but different, then
    // they aren't divisible. If they are the same, then the result 
    // I think is 1, but I'd have to write it out...
    // For now, we'll just return an error and give back the quotient
    // and remainder...
    //
    // TODO: Handle this edge case appropiately.
    if(p_len == q_len) {
        return -1;
    }

    // When polynomials are non-monic, we need the normalizer
    uint8_t normalizer = q[0];
    if(normalizer == 0)
        normalizer = 1;

    // Okay, this is the fancy synthetic division way directly adapted from 
    // the wikiversity page since it seems faster and more concise than 
    // the general synthetic division method.
    for(int i = 0; i < (p_len - (q_len - 1)); i++) {
        // Normalize the coefficient
        buffer_quotient[i] = gf8_div(buffer_quotient[i], normalizer);
        // Grab the current coefficient.
        uint8_t coef = buffer_quotient[i];
        // Avoid 0 since log(0) is undefined.
        if(coef != 0) { 
            // Skip first coefficient for the divisor.
            for(int j = 1; j < q_len; j++) {   
                // Zero doesnt really do anything
                if(q[j] != 0) {
                    // Do the actual math
                    buffer_quotient[i+j] ^= gf8_mul(q[j], coef); 
                } 
            }
        }
    }


    // The algorithm puts both the quotient and the remainder into 
    // buffer_quotient, so we need to split it out
    // We know the remainder is the degree as the divisor (which is length-1)
    // so we can grab that off buffer_quotient
    int seperator_index = p_len - (q_len - 1);
    int seperator_size = p_len - seperator_index;

    // Grab the remainder and delete it from the quotient
    int k = 1;
    for(int i = seperator_index; i < p_len; i++) {
        // Should never happen but.. you never know..
        if(k >= q_len) {
            return -1;
        }
        buffer_remainder[k] = buffer_quotient[i];
        buffer_quotient[i] = 0;
        k++;
    }

    // Shift the quotient buffer over so the polynomials line up like
    // they should. Zero out the higher order polynomials
    for(int i = p_len - 1; i >= 0; i--) {
        int j = i - seperator_size;
        if(j < 0) {
            buffer_quotient[i] = 0;
        } else {
            buffer_quotient[i] = buffer_quotient[j];
        }
    }

    // If we made it this far, everything succeeded!
    return 0;
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
