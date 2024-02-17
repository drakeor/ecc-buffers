cmake_minimum_required(VERSION 3.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(EccBuffers)

add_executable(ecc-buffer-tests 
    galois_field_8.c 
    tests/test_main.c 
    tests/galois_field_8_tests.c
    tests/unity/unity.c
)