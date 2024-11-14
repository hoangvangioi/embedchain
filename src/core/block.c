#include "block.h"

#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Block* create_block(int index, const char* previous_hash)
{
    Block* new_block = (Block*) calloc(1, sizeof(Block));
    new_block->index = index;
    strncpy(new_block->previous_hash, previous_hash, HASH_LENGTH - 1);
    new_block->previous_hash[HASH_LENGTH - 1] = '\0';

    char input[512];
    snprintf(input, sizeof(input), "%d%s", index, previous_hash);

    uint8_t hash[BLAKE3_OUT_LEN];
    blake3_hash(hash, (uint8_t*) input, strlen(input));
    for (int i = 0; i < BLAKE3_OUT_LEN; i++)
    {
        snprintf(&new_block->hash[i * 2], 3, "%02x", hash[i]);
    }

    return new_block;
}

void print_block(Block* block)
{
    printf("Block #%d\n", block->index);
    printf("Previous Hash: %s\n", block->previous_hash);
    printf("Transactions:\n");
    for (int i = 0; i < block->transaction_count; i++)
    {
        Transaction* tx = &block->transactions[i];
        printf("  %s -> %ld: %s\n", tx->sender, tx->timestamp, tx->data);
    }
    printf("Hash: %s\n\n", block->hash);
}

int verify_block(Block* block)
{
    uint8_t computedHash[BLAKE3_OUT_LEN];
    char    input[512];
    snprintf(input, sizeof(input), "%d%s", block->index, block->previous_hash);

    for (int i = 0; i < block->transaction_count; i++)
    {
        snprintf(input + strlen(input), sizeof(input) - strlen(input), "%s%lu%s", block->transactions[i].sender, block->transactions[i].timestamp, block->transactions[i].data);
    }

    blake3_hash(computedHash, (uint8_t*) input, strlen(input));
    char computedHashHex[HASH_LENGTH];
    for (int i = 0; i < BLAKE3_OUT_LEN; i++)
    {
        snprintf(&computedHashHex[i * 2], 3, "%02x", computedHash[i]);
    }

    return strncmp(computedHashHex, block->hash, HASH_LENGTH) == 0;
}
