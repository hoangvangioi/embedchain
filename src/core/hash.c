#include "hash.h"

#include "blake3.h"

void blake3_hash(uint8_t* hash, const uint8_t* data, size_t length)
{
    blake3_hasher hasher;
    blake3_hasher_init(&hasher);
    blake3_hasher_update(&hasher, data, length);
    blake3_hasher_finalize(&hasher, hash, BLAKE3_OUT_LEN);
}
