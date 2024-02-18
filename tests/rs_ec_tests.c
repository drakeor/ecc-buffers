#include "unity/unity.h"
#include "rs_ec_tests.h"
#include "../rs_ec.h"

#define BUFFER_SIZE 512

#define MESSAGE_SIZE 223
#define SYMBOL_SIZE 32

void rs_generator_polynomial_tests()
{
    uint8_t buffer[BUFFER_SIZE] = {0};
    uint8_t working_buffer[BUFFER_SIZE] = {0};

    // Trivial case
    int result = rs_generator_polynomial(buffer, working_buffer, 1);

    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(1, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(1, buffer[1]);

    // Slightly more complicated case
    result = rs_generator_polynomial(buffer, working_buffer, 2);

    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(1, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(3, buffer[1]);
    TEST_ASSERT_EQUAL_HEX8(2, buffer[2]);

    // Shamelessly taken from the wikiversity page
    // I didn't want to do the math myself
    result = rs_generator_polynomial(buffer, working_buffer, 4);

    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(1, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(0x0F, buffer[1]);
    TEST_ASSERT_EQUAL_HEX8(0x36, buffer[2]);
    TEST_ASSERT_EQUAL_HEX8(0x78, buffer[3]);
    TEST_ASSERT_EQUAL_HEX8(0x40, buffer[4]);
}


void rs_encode_tests()
{
    // This is the test case from the wikiversity page
    uint8_t buffer[BUFFER_SIZE] = {0};
    uint8_t working_buffer[BUFFER_SIZE] = {0};
    uint8_t generator_polynomial_buffer[BUFFER_SIZE] = {0};
    uint8_t message[] = { 
        0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06,
        0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96, 0x70, 0xec };

    rs_generator_polynomial(generator_polynomial_buffer, 
        working_buffer, 10);

    int result = rs_encode(buffer, working_buffer, 
        message, 16, 
        generator_polynomial_buffer, 10);

    // print out the buffer in hexidecimal
    for(int i = 0; i < 255; i++) {
        printf("%02X ", buffer[i]);
    }

    TEST_ASSERT_EQUAL_INT8(0, result);
}