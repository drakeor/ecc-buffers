#ifndef _RS_EC_H_
#define _RS_EC_H_

#include <stdint.h>

/*
    * Computes a generator polynomial for a Reed-Solomon code
    * @param buffer Needs to be at least twice the size 
    *       of generator_length 
    * @param working_buffer Needs to be at least twice the size 
    *       of generator_length
    * @param generator_length Size of the buffer to fill
    *   !!NOTE that this will create (generator_length - 1) symbols
    * @return 0 if the operation was successful, -1 otherwise
*/
int rs_generator_polynomial(uint8_t* buffer, 
    uint8_t* working_buffer, int generator_length);

/*
    * Encodes a message using a Reed-Solomon code
    * @param buffer Needs to be at least the size of
    *       (message_length + generator_length)
    * @param working_buffer Needs to be at least the size of
    *       (message_length + generator_length)
    * @param message Message to encode Needs to be at least the size of
    *       (message_length + generator_length)
    *       The area after the message should be zeroed out
    * @param message_length Length of the message
    * @param generator_polynomial Generator polynomial to use
    * @param generator_length Length of the generator polynomial
    * @return 0 if the operation was successful, -1 otherwise
    * 
    * @note The buffer will contain the message followed 
    *   by the RS code. Also note that the message_length and
    *   generator_length should be less than 256.
*/
int rs_encode(
    uint8_t* buffer, uint8_t* working_buffer,
    uint8_t* message, int message_length, 
    uint8_t* generator_polynomial, int generator_length);


int rs_calc_syndromes(
    uint8_t* buffer, uint8_t* message, 
    int message_length, int generator_length);

    
#endif