#ifndef HASH_H
#define HASH_H

#include "blake3.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define BLAKE3_OUT_LEN 32

#ifdef __cplusplus
extern "C"
{
#endif

    void blake3_hash(uint8_t* hash, const uint8_t* data, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* HASH_H */
