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
    int result = rs_generator_polynomial(buffer, working_buffer, 2);

    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(1, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(1, buffer[1]);

    // Slightly more complicated case
    result = rs_generator_polynomial(buffer, working_buffer, 3);

    TEST_ASSERT_EQUAL_INT8(0, result);
    TEST_ASSERT_EQUAL_HEX8(1, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(3, buffer[1]);
    TEST_ASSERT_EQUAL_HEX8(2, buffer[2]);

    // Shamelessly taken from the wikiversity page
    // I didn't want to do the math myself
    result = rs_generator_polynomial(buffer, working_buffer, 5);

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
    uint8_t message_buffer[BUFFER_SIZE] = {0};

    uint8_t message[] = { 
        0x40, 0xd2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06,
        0x27, 0x26, 0x96, 0xc6, 0xc6, 0x96, 0x70, 0xec };

    // The scenerio uses 10 symbols, which is a buffer of 11.
    int polynomialLength = 11;
    rs_generator_polynomial(generator_polynomial_buffer, 
        working_buffer, polynomialLength);

    // Copy message to message_buffer
    for(int i = 0; i < sizeof(message); i++) {
        message_buffer[i] = message[i];
    }

    // Do the actual encoding
    int result = rs_encode(buffer, working_buffer, 
        message_buffer, sizeof(message), 
        generator_polynomial_buffer, polynomialLength);

    TEST_ASSERT_EQUAL_INT8(0, result);

    // Resulting message from wikiversity
    uint8_t expected_output[] = {
        // Original Message
        0x40, 0xD2, 0x75, 0x47, 0x76, 0x17, 0x32, 0x06,
        0x27, 0x26 ,0x96, 0xC6, 0xC6, 0x96, 0x70, 0xEC,
        // ECC Code
        0xBC, 0x2A, 0x90, 0x13, 0x6B, 0xAF, 0xEF, 0xFD,
        0x4B, 0xE0
    };

    // Compare the output to the buffer
    for(int i = 0; i < sizeof(expected_output); i++) {
        TEST_ASSERT_EQUAL_HEX8(expected_output[i], buffer[i]);
    }
}

void rs_encode_tests_2()
{
     // This is the test case I came up with
    uint8_t buffer[BUFFER_SIZE] = {0};
    uint8_t working_buffer[BUFFER_SIZE] = {0};
    uint8_t generator_polynomial_buffer[BUFFER_SIZE] = {0};
    uint8_t message_buffer[BUFFER_SIZE] = {0};

    uint8_t message[] = { 0x55, 0x44, 0x33, 0x22, 0x11 };

    // The scenerio uses 14 symbols, which is a buffer of 15.
    int polynomialLength = 15;
    rs_generator_polynomial(generator_polynomial_buffer, 
        working_buffer, polynomialLength);

    // Copy message to message_buffer
    for(int i = 0; i < sizeof(message); i++) {
        message_buffer[i] = message[i];
    }

    // Do the actual encoding
    int result = rs_encode(buffer, working_buffer, 
        message_buffer, sizeof(message), 
        generator_polynomial_buffer, polynomialLength);

    TEST_ASSERT_EQUAL_INT8(0, result);

    // Resulting message from wikiversity
    uint8_t expected_output[] = {
        // Original Message
        0x55, 0x44, 0x33, 0x22, 0x11, 
        // ECC Code
        0x52, 0x2C, 0xF6, 0x29, 0xEE, 
        0x55, 0xE7, 0x4A, 0x69, 0xD0, 
        0x03, 0x7A, 0xE0, 0x86
    };

    // Compare the output to the buffer
    for(int i = 0; i < sizeof(expected_output); i++) {
        TEST_ASSERT_EQUAL_HEX8(expected_output[i], buffer[i]);
    }
}

void rs_calc_syndromes_tests()
{
    // Message from the wikipedia page
    uint8_t buffer[BUFFER_SIZE] = {0};
    uint8_t message[] = {
        // Original Message
        0x55, 0x44, 0x33, 0x22, 0x11, 
        // ECC Code
        0x52, 0x2C, 0xF6, 0x29, 0xEE, 
        0x55, 0xE7, 0x4A, 0x69, 0xD0, 
        0x03, 0x7A, 0xE0, 0x86
    };
    int message_size = sizeof(message);

    // The scenerio uses 14 symbols, which is a buffer of 15.
    int generator_length = 15;
    int result = rs_calc_syndromes(buffer, message, 
        message_size, generator_length);

    TEST_ASSERT_EQUAL_INT8(0, result);

    // Check the buffer is all 0s
    for(int i = 0; i < generator_length; i++) {
        TEST_ASSERT_EQUAL_HEX8(0, buffer[i]);
    }
}


void rs_calc_syndromes_tests_2()
{
    // Message from the wikipedia page
    uint8_t buffer[BUFFER_SIZE] = {0};
    uint8_t message[] = {
        // Original Message 
        0x55, 0x44, 0x33, 0x22, 0x11, 
        // ECC Code
        0x52, 0x2C, 0xF6, 0x29, 0xEE, 
        0x55, 0xE7, 0x4A, 0x69, 0xD0, 
        0x03, 0x7A, 0xE0, 0x86
    };
    int message_size = sizeof(message);

    // Corrupt the first bit with a zero
    message[0] = 0;

    // The scenerio uses 14 symbols, which is a buffer of 15.
    int generator_length = 15;
    int result = rs_calc_syndromes(buffer, message, 
        message_size, generator_length);
    TEST_ASSERT_EQUAL_INT8(0, result);

    uint8_t expected_syndrome[] = {
        0x00, 0x55, 0xfc, 0x6e, 0xdd, 
        0x8a, 0x2c, 0x08, 0x75, 0x35, 
        0xba, 0x0f, 0xb6, 0xce, 0x17
    };

    // Check the buffer matches expected_syndrome
    for(int i = 0; i < generator_length; i++) {
        TEST_ASSERT_EQUAL_HEX8(expected_syndrome[i], buffer[i]);
    }
}