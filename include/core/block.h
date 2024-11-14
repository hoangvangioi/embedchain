#ifndef BLOCK_H
#define BLOCK_H

#include "transaction.h"

#define HASH_LENGTH 65

typedef struct Block
{
    int           index;
    char          previous_hash[HASH_LENGTH];
    Transaction   transactions[MAX_TRANSACTIONS];
    int           transaction_count;
    char          hash[HASH_LENGTH];
    struct Block* next;
} Block;

#ifdef __cplusplus
extern "C"
{
#endif

    Block* create_block(int index, const char* previous_hash);
    void   print_block(Block* block);
    int    verify_block(Block* block);

#ifdef __cplusplus
}
#endif

#endif /* BLOCK_H */
