#include "rs_ec.h"
#include "galois_field_8.h"
#include <stdio.h>

int rs_generator_polynomial(uint8_t* buffer, 
    uint8_t* working_buffer, int generator_length)
{

    // Zero out both buffers
    for(int i = 0; i < generator_length; i++) {
        buffer[i] = 0;
        working_buffer[i] = 0;
    }

    // Set the first element of the generator polynomial to 1
    buffer[0] = 1;
    uint8_t q[2] = {1, 0};

    // We flip between the two buffers since gf8_poly_mul
    // modifies data in place.
    int current_buffer = 0;
    int polyres = 0;

    // Remember, number of symbols is generator_length - 1
    int number_of_symbols = generator_length - 1;
    for(int i = 0; i < number_of_symbols; i++) {
        // Populate our new value of q
        q[1] = gf8_pow(2, i);
        
        if(current_buffer == 0) {
            // Multiply the current buffer by q and store the result in the
            // working buffer
            polyres = gf8_poly_mul(working_buffer, buffer, 
                q, i + 1, 2);
            current_buffer = 1;
        } else {
            // Multiply the working buffer by q and store the result in the
            // current buffer
            polyres = gf8_poly_mul(buffer, working_buffer, 
                q, i + 1, 2);
            current_buffer = 0;
        }
        // Make sure the multiplication was successful
        if(polyres != 0) {
            return -1;
        }
    }

    // If we ended on the working buffer, copy it to the buffer
    if(current_buffer == 1) {
        for(int i = 0; i < generator_length; i++) {
            buffer[i] = working_buffer[i];
        }
    }
    return 0;
}

int rs_encode(
    uint8_t* buffer, uint8_t* working_buffer,
    uint8_t* message, int message_length, 
    uint8_t* generator_polynomial, int generator_length)
{
    // We can't encode a message with a buffer size over 255
    int buffer_size = message_length + generator_length - 1;
    if(buffer_size > 255) {
        return -1;
    }

    // Zero out the buffers
    for(int i = 0; i < buffer_size; i++) {
        buffer[i] = 0;
        working_buffer[i] = 0;
    }

    // The remainder of the division will be the RS Code
    // We store it in working buffer for now.
    int result = gf8_poly_div(buffer, working_buffer, 
        message, generator_polynomial, 
        buffer_size, generator_length);


    // Make sure it succeeded
    if(result != 0) {
        return -1;
    }

    // Copy the mesage and the remainder to the buffer
    // We skip the first term in the remainder since it's 0
    for(int i = 0; i < buffer_size; i++) {
        if(i < message_length) {
            buffer[i] = message[i];
        } else {
            buffer[i] = working_buffer[(i - message_length)+1];
        }
    }
    return 0;
}