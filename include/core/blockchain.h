#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "block.h"

typedef struct Blockchain
{
    Block* head;
    Block* tail;
    int    length;
} Blockchain;

#ifdef __cplusplus
extern "C"
{
#endif

    Blockchain* create_blockchain();
    void        add_block(Blockchain* blockchain, Block* new_block);
    void        print_blockchain(Blockchain* blockchain);
    int         verify_blockchain(Blockchain* blockchain);
    void        free_blockchain(Blockchain* blockchain);

#ifdef __cplusplus
}
#endif

#endif /* BLOCKCHAIN_H */
