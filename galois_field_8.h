/*
 * Represents field operations in GF(2^8)
 */

#ifndef _GALOIS_FIELD_8_H_
#define _GALOIS_FIELD_8_H_

#include <stdint.h>

/*
    * Initializes the lookup tables for multiplications in GF(2^8)
    * @return 0 if initialization was successful, -1 otherwise
*/
int gf8_init();

/*
    * Adds two numbers in GF(2^8)
    * @param a First number
    * @param b Second number
    * @return Sum of a and b in GF(2^8)
*/
uint8_t gf8_add(uint8_t a, uint8_t b);


/*
    * Subtracts two numbers in GF(2^8)
    * @param a First number
    * @param b Second number
    * @return Difference of a and b in GF(2^8)
*/
uint8_t gf8_sub(uint8_t a, uint8_t b);


/*
    * Multiplies two numbers in GF(2^8)
    * @param a First number
    * @param b Second number
    * @return Product of a and b in GF(2^8)
*/
uint8_t gf8_mul(uint8_t a, uint8_t b);
uint8_t gf8_mul_nolut(uint8_t a, uint8_t b);

/*
    * Divides two numbers in GF(2^8)
    * @param a First number. Dividend
    * @param b Second number. Divisor
    * @return Quotient of a and b in GF(2^8)
*/
uint8_t gf8_div(uint8_t a, uint8_t b);

/*
    * Raises a number to a power in GF(2^8)
    * @param a Base
    * @param pow Exponent
    * @return a raised to the power of pow in GF(2^8)
*/
uint8_t gf8_pow(uint8_t a, uint8_t pow);

/*
    * Inverts a number in GF(2^8)
    * @param a Number to invert
    * @return Inverse of a in GF(2^8)
*/
uint8_t gf8_inv(uint8_t a);

/*
    * Multiplies a number by a scalar in GF(2^8)
    * @param buffer The buffer should be at least the size
    *       of the polynomial
    *       [Ex: 2x^2 + 2x^1 + 2x^0]
    * @param p Polynomial to scale [Ex: x^2 + x^1 + x^0]
    * @param scale Scalar to multiply by [Ex: 2]
    * @param length Length of the polynomial p
    * @return 0 if the operation was successful, -1 otherwise
*/
int gf8_poly_scale(uint8_t* buffer, uint8_t* p, uint8_t scale, uint8_t p_len);

/*
    * Adds two polynomials in GF(2^8)
    * @param buffer The buffer should be at least the size,
    *       of both polynomials.
    *       [Ex: x^3 + x^2 + x^1 + x^0]
    * @param p First polynomial [Ex: x^3 + x^2 + x^1 + x^0]
    * @param q Second polynomial [Ex: x^2 + x^1 + x^0]
    * @param p_len Length of the polynomial p
    * @param q_len Length of the polynomial q
    * @return 0 if the operation was successful, -1 otherwise
*/
int gf8_poly_add(uint8_t* buffer, uint8_t* p, uint8_t* q, 
    uint8_t p_len, uint8_t q_len);

/*
    * Multiplies two polynomials in GF(2^8)
    * @param buffer The buffer should be at least the size
    *       of the sum of the lengths of both polynomials
    *       [Ex: x^5 + x^4 + .. +x^0]
    * @param p First polynomial [Ex: x^3 + x^2 + x^1 + x^0]
    * @param q Second polynomial [Ex: x^2 + x^1 + x^0]
    * @param p_len Length of the polynomial p
    * @param q_len Length of the polynomial q
    * @return 0 if the operation was successful, -1 otherwise
*/
int gf8_poly_mul(uint8_t* buffer, uint8_t* p, uint8_t* q, 
    uint8_t p_len, uint8_t q_len);

/*
    * Divides two polynomials in GF(2^8).
    * We expect polynomials to go from highest to lowest order like normal.
    * 
    * @param buffer_quotient The buffer should be at least the size 
    *       of the p polynomial (dividend)
    * @param buffer_remainder The buffer should be at least the size 
    *       of the q polynomial (divisor)
    * @param p First polynomial (Dividend). This should be bigger, or the 
*           same size as the divisor.
    * @param q Second polynomial (Divisor). This should be smaller
    *       than the dividend
    * @param p_len Length of the polynomial p
    * @param q_len Length of the polynomial q
    * @return 0 if the operation was successful, -1 otherwise
*/
int gf8_poly_div(uint8_t* buffer_quotient, uint8_t* buffer_remainder,
     uint8_t* p, uint8_t* q, uint8_t p_len, uint8_t q_len);

/*
    * Evaluates a polynomial in GF(2^8) and returns a scalar
    * @param p Polynomial to evaluate
    * @param x Value to evaluate the polynomial at
    * @param p_len Length of the polynomial
    * @return 0 if the operation was successful, -1 otherwise
*/
uint8_t gf8_poly_eval(uint8_t* p, uint8_t x, uint8_t p_len);

#endif