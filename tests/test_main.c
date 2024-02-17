#include "unity/unity.h"
#include "galois_field_8_tests.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_function_should_doBlahAndBlah(void) {
    TEST_ASSERT_TRUE(1);
}

void test_function_should_doAlsoDoBlah(void) {
    TEST_ASSERT_TRUE(0);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_function_should_doBlahAndBlah);
    RUN_TEST(test_function_should_doAlsoDoBlah);
    return UNITY_END();
}