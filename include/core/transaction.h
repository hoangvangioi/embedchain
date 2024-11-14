#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stddef.h>
#include <stdint.h>

#define MAX_DATA_LENGTH 256
#define MAX_TRANSACTIONS 100
#define SENDER_RECEIVER_LENGTH 50

typedef struct Transaction
{
    char     sender[SENDER_RECEIVER_LENGTH];
    uint64_t timestamp;
    char     data[MAX_DATA_LENGTH];
} Transaction;

#ifdef __cplusplus
extern "C"
{
#endif

    int  verify_transaction(const Transaction* transaction);
    void add_transaction(Transaction* transactions, int* count, const char* sender, uint64_t timestamp, const char* data);

#ifdef __cplusplus
}
#endif

#endif /* TRANSACTION_H */
