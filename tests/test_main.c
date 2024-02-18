#include "unity/unity.h"
#include "galois_field_8_tests.h"


int main()
{
    UNITY_BEGIN();
    
    // Unit tests on galois field operations
    RUN_TEST(gf8_add_tests);
    RUN_TEST(gf8_sub_tests);

    RUN_TEST(gf8_mul_tests);
    RUN_TEST(gf8_mul_lut_tests);
    
    RUN_TEST(gf8_div_tests);
    RUN_TEST(gf8_muldiv_exhaustive_tests);

    return UNITY_END();
}