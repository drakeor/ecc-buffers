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

/*
    * Divides two numbers in GF(2^8)
    * @param a First number
    * @param b Second number
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
    * @param buffer The buffer should be the same size, or larger, than the polynomial
    * @param p Polynomial to scale
    * @param scale Scalar to multiply by
    * @param length Length of the polynomial
    * @return 0 if the operation was successful, -1 otherwise
*/
int gf8_poly_scale(uint8_t* buffer, uint8_t* p, uint8_t scale, uint8_t length);

/*
    * Adds two polynomials in GF(2^8)
    * @param buffer The buffer should be the same size, or larger, than both polynomials
    * @param p First polynomial
    * @param q Second polynomial
    * @param length Length of the polynomials
    * @return 0 if the operation was successful, -1 otherwise
*/
int gf8_poly_add(uint8_t* buffer, uint8_t* p, uint8_t* q, uint8_t length);

/*
    * Multiplies two polynomials in GF(2^8)
    * @param buffer The buffer should be the same size, or larger, than both polynomials
    * @param p First polynomial
    * @param q Second polynomial
    * @param length Length of the polynomials
    * @return 0 if the operation was successful, -1 otherwise
*/
int gf8_poly_mul(uint8_t* buffer, uint8_t* p, uint8_t* q, uint8_t length);

/*
    * Divides two polynomials in GF(2^8)
    * @param buffer The buffer should be the same size, or larger, than both polynomial
    * @param p First polynomial (Dividend)
    * @param q Second polynomial (Divisor)
    * @param length Length of the polynomials
    * @return 0 if the operation was successful, -1 otherwise
*/
int gf8_poly_div(uint8_t* buffer, uint8_t* p, uint8_t* q, uint8_t length);

/*
    * Evaluates a polynomial in GF(2^8) to produce a particular value of x
    * @param buffer The buffer should be the same size, or larger, than the polynomial
    * @param p Polynomial to evaluate
    * @param x Value to evaluate the polynomial at
    * @param length Length of the polynomial
    * @return 0 if the operation was successful, -1 otherwise
*/
int gf8_poly_eval(uint8_t* buffer, uint8_t* p, uint8_t x, uint8_t length);

#endif