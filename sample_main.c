#include <stdint.h>
#include <stdio.h>
#include "rs_ec.h"
#include <time.h>

#define BUFFER_SIZE 384
//#define MESSAGE_SIZE 223
//#define SYMBOL_SIZE 32

#define MESSAGE_SIZE 239
#define SYMBOL_SIZE 16

int main()
{
    // All the memory we need for the entire application
    uint8_t buffer[BUFFER_SIZE] = {0};
    uint8_t working_buffer[BUFFER_SIZE] = {0};
    uint8_t generator_polynomial_buffer[BUFFER_SIZE] = {0};
    uint8_t message_buffer[BUFFER_SIZE] = {0};

    int rs_chunk_size = MESSAGE_SIZE + SYMBOL_SIZE - 1;

    // Copy the message to the message buffer
    uint8_t message[] = "Test Message!";
    for(int i = 0; i < (int)sizeof(message); i++) {
        message_buffer[i] = message[i];
    }

    // Generate the generator polynomial
    printf("Generating Generator Polynomial\n");
    rs_generator_polynomial(generator_polynomial_buffer, 
        working_buffer, SYMBOL_SIZE);

    // Do the actual encoding
    int result = rs_encode(buffer, working_buffer, 
        message_buffer, MESSAGE_SIZE, 
        generator_polynomial_buffer, SYMBOL_SIZE);
    
    if(result != 0) {
        printf("encoding failed.\n");
        return 1;
    }

    // Print the buffer
    printf("Encoded Message: ");
    for(int i = 0; i < rs_chunk_size; i++) {
        printf("%02X ", buffer[i]);
    }

    // Calculate the syndromes
    printf("\nCalculating Syndromes\n");
    result = rs_calc_syndromes(working_buffer, buffer, 
        rs_chunk_size, SYMBOL_SIZE);
    if(result != 0) {
        printf("syndrome calculation failed.\n");
        return 1;
    }

    // Print the symdromes
    printf("Syndromes: ");
    for(int i = 0; i < SYMBOL_SIZE; i++) {
        printf("%02X ", working_buffer[i]);
    }
    printf("\n");

    // Calculate if there was an error
    printf("Checking for Errors\n");
    result = rs_check_if_error(working_buffer, SYMBOL_SIZE);
    if(result != 0) {
        printf("Error detected.\n");
    } else {
        printf("No errors detected.\n");
    }

    // Introduce an error
    buffer[0] = 0x00;

    // Calculate the syndromes
    printf("Calculating Syndromes\n");
    result = rs_calc_syndromes(working_buffer, buffer, 
        rs_chunk_size, SYMBOL_SIZE);
    if(result != 0) {
        printf("syndrome calculation failed.\n");
        return 1;
    }

    // Print the symdromes
    printf("Syndromes: ");
    for(int i = 0; i < SYMBOL_SIZE; i++) {
        printf("%02X ", working_buffer[i]);
    }
    printf("\n");

    // Calculate if there was an error
    {
        printf("Checking for Errors\n");
        result = rs_check_if_error(working_buffer, SYMBOL_SIZE);
        if(result != 0) {
            printf("Error detected.\n");
        } else {
            printf("No errors detected.\n");
        }

        // Time the encoding
        printf("Benchmarking rs_encode..\n");
        int sample_size = 25000;
        int data_size = MESSAGE_SIZE + SYMBOL_SIZE;
        clock_t begin = clock();
        for(int i = 0; i < sample_size; i++) {
            rs_encode(buffer, working_buffer, 
                message_buffer, MESSAGE_SIZE, 
                generator_polynomial_buffer, SYMBOL_SIZE);
        }
        clock_t end = clock();
        
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        double time_per_encode = time_spent / sample_size;
        double data_per_second = data_size / time_per_encode;
        double data_per_second_mbps = data_per_second / 1000000;

        printf("Time spent: %f s\n", time_spent);
        printf("Data per second encoded: %f MB/s\n", data_per_second_mbps);
    }

    // Restore the buffer
    buffer[0] = 'T';

    // Calculate if there was an error
    {
        printf("Checking for Errors\n");
        result = rs_check_if_error(working_buffer, SYMBOL_SIZE);
        if(result != 0) {
            printf("Error detected.\n");
        } else {
            printf("No errors detected.\n");
        }

        // Time the encoding
        printf("Benchmarking rs_calc_syndromes..\n");
        int sample_size = 25000;
        int data_size = MESSAGE_SIZE + SYMBOL_SIZE;
        clock_t begin = clock();
        for(int i = 0; i < sample_size; i++) {
            rs_calc_syndromes(working_buffer, buffer, 
                rs_chunk_size, SYMBOL_SIZE);
        }
        clock_t end = clock();
        
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        double time_per_encode = time_spent / sample_size;
        double data_per_second = data_size / time_per_encode;
        double data_per_second_mbps = data_per_second / 1000000;

        printf("Time spent: %f s\n", time_spent);
        printf("Data per second encoded: %f MB/s\n", data_per_second_mbps);
    }

    return 0;
}