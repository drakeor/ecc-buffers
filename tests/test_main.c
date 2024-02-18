#include "unity/unity.h"
#include "galois_field_8_tests.h"
#include "galois_field_8_poly_tests.h"
#include "rs_ec_tests.h"

int main()
{
    UNITY_BEGIN();
    

    // Unit tests on galois field operations
    ////
    RUN_TEST(gf8_add_tests);
    RUN_TEST(gf8_sub_tests);

    RUN_TEST(gf8_mul_tests);
    RUN_TEST(gf8_mul_lut_tests);
    
    RUN_TEST(gf8_div_tests);
    RUN_TEST(gf8_muldiv_exhaustive_tests);

    RUN_TEST(gf8_inv_tests);
    RUN_TEST(gf8_pow_tests);


    // Unit tests on galois polynomials
    ////
    RUN_TEST(gf8_poly_scale_tests);

    RUN_TEST(gf8_poly_add_tests);

    RUN_TEST(gf8_poly_mul_tests);
    RUN_TEST(gf8_poly_mul_long_tests);

    RUN_TEST(gf8_poly_div_tests_simple);
    RUN_TEST(gf8_poly_div_tests);
    RUN_TEST(gf8_poly_div_tests2);

    RUN_TEST(gf8_poly_eval_tests);


    // RS Error Correction tests
    ////    
    RUN_TEST(rs_generator_polynomial_tests);
    RUN_TEST(rs_encode_tests);

    return UNITY_END();
}