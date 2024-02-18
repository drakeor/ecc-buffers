#include "unity/unity.h"
#include "galois_field_8_poly_tests.h"
#include "../galois_field_8.h"

#define BUFFER_SIZE 0xFF

void gf8_poly_scale_tests()
{
    // This is straightforward
    uint8_t buffer[BUFFER_SIZE] = {0};
    uint8_t p[4] = { 1, 2, 3, 4};

    int result = gf8_poly_scale(buffer, p, 2, 4);

    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(2, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(4, buffer[1]);
    TEST_ASSERT_EQUAL_HEX8(6, buffer[2]);
    TEST_ASSERT_EQUAL_HEX8(8, buffer[3]);

    // Slightly more complicated example to test overflow
    result = gf8_poly_scale(buffer, p, 255, 4);

    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(255, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(227, buffer[1]);
    TEST_ASSERT_EQUAL_HEX8(28, buffer[2]);
    TEST_ASSERT_EQUAL_HEX8(219, buffer[3]);
}

void gf8_poly_add_tests()
{
    uint8_t buffer[BUFFER_SIZE] = {0};
    uint8_t p[3] = { 1, 255, 40};
    uint8_t q[2] = { 20, 50};

    // Add p and q together
    int result = gf8_poly_add(buffer, p, q, 3, 2);

    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(1, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(235, buffer[1]);
    TEST_ASSERT_EQUAL_HEX8(26, buffer[2]);

    // Addition is communitive, swap p and q.
    result = gf8_poly_add(buffer, q, p, 2, 3);

    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(1, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(235, buffer[1]);
    TEST_ASSERT_EQUAL_HEX8(26, buffer[2]);
}

void gf8_poly_mul_tests()
{
    // Trivial polynomial example
    uint8_t buffer[BUFFER_SIZE] = {0};
    uint8_t p[2] = { 1, 101};
    uint8_t q[1] = { 42 };

    int result = gf8_poly_mul(buffer, p, q, 2, 1);
    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(42, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(249, buffer[1]);

    // Multiplication is communitive, swap p and q
    result = gf8_poly_mul(buffer, q, p, 1, 2);
    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(42, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(249, buffer[1]);

}

void gf8_poly_mul_long_tests()
{
    // This is a longer multiplication test.
    // Only one since I'm testing against doing it by hand 
    // and it's tedious
    uint8_t buffer[BUFFER_SIZE] = {0};
    uint8_t p[3] = { 55, 33, 200 };
    uint8_t q[2] = { 15, 243 };

    int result = gf8_poly_mul(buffer, p, q, 3, 2);
    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(32, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(145, buffer[1]);
    TEST_ASSERT_EQUAL_HEX8(180, buffer[2]);
    TEST_ASSERT_EQUAL_HEX8(241, buffer[3]);
}

void gf8_poly_div_tests_simple()
{
    uint8_t buffer_quotient[BUFFER_SIZE] = {0};
    uint8_t buffer_remainder[BUFFER_SIZE] = {0};

    // Much easier to test and theoretically should work.
    uint8_t p[5] = { 1, 0, 1, 1, 1};
    uint8_t q[3] = { 1, 0, 1 };

    int result = gf8_poly_div(buffer_quotient, buffer_remainder, 
        p, q, 5, 3);

    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(0, buffer_quotient[0]);
    TEST_ASSERT_EQUAL_HEX8(0, buffer_quotient[1]);
    TEST_ASSERT_EQUAL_HEX8(1, buffer_quotient[2]);
    TEST_ASSERT_EQUAL_HEX8(0, buffer_quotient[3]);
    TEST_ASSERT_EQUAL_HEX8(0, buffer_quotient[4]);

    TEST_ASSERT_EQUAL_HEX8(0, buffer_remainder[0]);
    TEST_ASSERT_EQUAL_HEX8(1, buffer_remainder[1]);
    TEST_ASSERT_EQUAL_HEX8(1, buffer_remainder[2]);
}

void gf8_poly_div_tests()
{
    uint8_t buffer_quotient[BUFFER_SIZE] = {0};
    uint8_t buffer_remainder[BUFFER_SIZE] = {0};

    // Shamelessly lifted from the wikiversity page
    // I didn't do this one myself
    uint8_t p[4] = { 32, 145, 180, 241 };
    uint8_t q[2] = { 15, 243 };

    int result = gf8_poly_div(buffer_quotient, buffer_remainder, 
        p, q, 4, 2);

    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(0, buffer_quotient[0]);
    TEST_ASSERT_EQUAL_HEX8(55, buffer_quotient[1]);
    TEST_ASSERT_EQUAL_HEX8(33, buffer_quotient[2]);
    TEST_ASSERT_EQUAL_HEX8(200, buffer_quotient[3]);

}

void gf8_poly_div_tests2()
{
    uint8_t buffer_quotient[BUFFER_SIZE] = {0};
    uint8_t buffer_remainder[BUFFER_SIZE] = {0};

    // Shamelessly lifted from the wikiversity page
    // I didn't do this one myself
    uint8_t p[7] = { 0x12, 0x34, 0x56, 0x00, 0x00, 0x00, 0x00 };
    uint8_t q[5] = { 0x01, 0x0F, 0x36, 0x78, 0x40 };

    int result = gf8_poly_div(buffer_quotient, buffer_remainder, 
        p, q, 7, 5);

    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(0, buffer_quotient[0]);
    TEST_ASSERT_EQUAL_HEX8(0, buffer_quotient[1]);
    TEST_ASSERT_EQUAL_HEX8(0, buffer_quotient[2]);
    TEST_ASSERT_EQUAL_HEX8(0, buffer_quotient[3]);
    TEST_ASSERT_EQUAL_HEX8(0x12, buffer_quotient[4]);
    TEST_ASSERT_EQUAL_HEX8(0xDA, buffer_quotient[5]);
    TEST_ASSERT_EQUAL_HEX8(0xDF, buffer_quotient[6]);

}