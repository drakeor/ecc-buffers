cmake_minimum_required(VERSION 3.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(EccBuffers)

add_executable(ecc-buffer-tests 
    galois_field_8.c 
    rs_ec.c
    tests/test_main.c 
    tests/unity/unity.c
    tests/galois_field_8_tests.c
    tests/galois_field_8_poly_tests.c
    tests/rs_ec_tests.c
)

add_executable(ecc-sample-app
    galois_field_8.c 
    rs_ec.c
    sample_main.c 
)