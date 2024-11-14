#include "blockchain.h"

#include <stdio.h>
#include <stdlib.h>

Blockchain* create_blockchain()
{
    Blockchain* blockchain = (Blockchain*) calloc(1, sizeof(Blockchain));
    return blockchain;
}

void add_block(Blockchain* blockchain, Block* new_block)
{
    if (!blockchain->head)
    {
        blockchain->head = new_block;
    }
    else
    {
        blockchain->tail->next = new_block;
    }
    blockchain->tail = new_block;
    blockchain->length++;
}

void print_blockchain(Blockchain* blockchain)
{
    for (Block* current = blockchain->head; current != NULL; current = current->next)
    {
        print_block(current);
    }
}

int verify_blockchain(Blockchain* blockchain)
{
    Block* current = blockchain->head;
    while (current != NULL)
    {
        if (!verify_block(current))
        {
            return 0; /* Invalid block found */
        }
        current = current->next;
    }
    return 1; /* All blocks are valid */
}

void free_blockchain(Blockchain* blockchain)
{
    Block* current = blockchain->head;
    while (current)
    {
        Block* next = current->next;
        free(current);
        current = next;
    }
    free(blockchain);
}
