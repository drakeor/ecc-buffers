#ifndef ECC_BUFFERS_H_
#define ECC_BUFFERS_H_

// If we're included in the linux kernel, let's include it's uint8_t type.
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

// Unknown. Replace with something valid / meaningful.
typedef uint8_t ecc_buffer_t;

// Probably should be an opaque type.
typedef uint8_t data_t;

int ecc_buffer_init(ecc_buffer_t *buffer, uint64_t size);
int ecc_buffer_free(ecc_buffer_t *buffer);
int ecc_buffer_set(ecc_buffer_t *buffer, data_t *data, uint64_t size);
int ecc_buffer_get(ecc_buffer_t *buffer, data_t *data, uint64_t size);

#endif // ECC_BUFFERS_H_