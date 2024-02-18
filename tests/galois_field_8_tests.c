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
    // Adding in GF(2) is isomorphic to XOR
    TEST_ASSERT_EQUAL_INT8(8, gf8_add(15, 7));
    TEST_ASSERT_EQUAL_INT8(8, gf8_add(7, 15));
    TEST_ASSERT_EQUAL_INT8(43, gf8_add(255, 212));
    TEST_ASSERT_EQUAL_INT8(254, gf8_add(1, 255));
}

void gf8_sub_tests() { 
    // Simple enough tests
    // Subtracting in GF(2) is isomorphic to XOR
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
    TEST_ASSERT_EQUAL_HEX8(0xB7, gf8_mul_nolut(0xF0, 0xF0));
}

void gf8_mul_lut_tests() {
    // Small enough that we can do an exhaustive test
    // Make sure the lut table has all the expected entries and
    // agrees with nolut.
    for(uint8_t a = 0; a < 0xFF; a++)
    {
        for(uint8_t b = 0; b < 0xFF; b++)
        {
            // Compare lut against nolut
            uint8_t val_a = gf8_mul_nolut(a, b);
            uint8_t val_b = gf8_mul(a, b);
            TEST_ASSERT_EQUAL_HEX8(val_a, val_b);
        }
    }
}

void gf8_div_tests() { 

    // Identities
    TEST_ASSERT_EQUAL_HEX8(0xFF, gf8_div(0xFF, 0x01));
    TEST_ASSERT_EQUAL_HEX8(0xFD, gf8_div(0x01, 0xFF));

    // Zeroes
    TEST_ASSERT_EQUAL_HEX8(0x00, gf8_div(0xFF, 0x00));
    TEST_ASSERT_EQUAL_HEX8(0x00, gf8_div(0x00, 0xFF));
    TEST_ASSERT_EQUAL_HEX8(0x00, gf8_div(0x00, 0x00));

    // Division as opposite of ones on top
    TEST_ASSERT_EQUAL_HEX8(0xFF, gf8_div(0xE2, 0xFF));
    TEST_ASSERT_EQUAL_HEX8(0x40, gf8_div(0x39, 0x52));
    TEST_ASSERT_EQUAL_HEX8(0x52, gf8_div(0x39, 0x40));
    TEST_ASSERT_EQUAL_HEX8(0xF0, gf8_div(0xB7, 0xF0));

}

void gf8_muldiv_exhaustive_tests() {
    // Small enough that we can do an exhaustive test

    for(uint8_t a = 0; a < 0xFF; a++)
    {
        for(uint8_t b = 0; b < 0xFF; b++)
        {
                // We run the operations on against the 
                // following equations:
                //              r = a*b
                // and          a = r*b^-1
                // to show that   
                //              a = a
                // in fact does hold.
            uint8_t r = gf8_mul(a, b);
            uint8_t a_new = gf8_div(r, b);

            // Dividing by zero is undefined.. but we expect zero,
            // not the original a
            uint8_t a_to_test = a;
            if(b == 0) {
                a_to_test = 0;
            }

            TEST_ASSERT_EQUAL_HEX8(a_to_test, a_new);
        }
    }
}