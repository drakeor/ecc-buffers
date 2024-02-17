#include "unity/unity.h"
#include "galois_field_8_tests.h"
#include "../galois_field_8.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}


void gf8_add_tests() { 
    // Simple enough tests
    TEST_ASSERT_EQUAL_INT8(8, gf8_add(15, 7));
    TEST_ASSERT_EQUAL_INT8(8, gf8_add(7, 15));
    TEST_ASSERT_EQUAL_INT8(43, gf8_add(255, 212));
    TEST_ASSERT_EQUAL_INT8(254, gf8_add(1, 255));
}

void gf8_sub_tests() { 
    // Simple enough tests
    TEST_ASSERT_EQUAL_INT8(15, gf8_sub(8, 7));
    TEST_ASSERT_EQUAL_INT8(15, gf8_sub(7, 8));
    TEST_ASSERT_EQUAL_INT8(10, gf8_sub(52, 62));
    TEST_ASSERT_EQUAL_INT8(10, gf8_sub(62, 52));
}

void gf8_mul_tests() { 
    // We test the nolut here since we test the 
    // lut table integrity later.

    // Small ones that won't trigger modulo
    TEST_ASSERT_EQUAL_HEX8(0x0A, gf8_mul_nolut(0x02, 0x05));

    // Identities
    TEST_ASSERT_EQUAL_HEX8(0xFF, gf8_mul_nolut(0xFF, 0x01));
    TEST_ASSERT_EQUAL_HEX8(0xFF, gf8_mul_nolut(0x01, 0xFF));

    // Mins, maxes, and zeroes
    TEST_ASSERT_EQUAL_HEX8(0xE2, gf8_mul_nolut(0xFF, 0xFF));
    TEST_ASSERT_EQUAL_HEX8(0x00, gf8_mul_nolut(0xFF, 0x00));
    TEST_ASSERT_EQUAL_HEX8(0x00, gf8_mul_nolut(0x00, 0xFF));
    TEST_ASSERT_EQUAL_HEX8(0x00, gf8_mul_nolut(0x00, 0x00));

    // Ones that trigger modulos
    TEST_ASSERT_EQUAL_HEX8(0x39, gf8_mul_nolut(0x40, 0x52));
    TEST_ASSERT_EQUAL_HEX8(0x39, gf8_mul_nolut(0x52, 0x40));
}

void gf8_div_tests() { 

}