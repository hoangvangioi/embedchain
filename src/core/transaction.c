#include "transaction.h"

#include <stdio.h>
#include <string.h>

int verify_transaction(const Transaction* transaction)
{
    return strlen(transaction->sender) > 0 && transaction->timestamp > 0 && strlen(transaction->data) > 0;
}

void add_transaction(Transaction* transactions, int* count, const char* sender, uint64_t timestamp, const char* data)
{
    if (*count < MAX_TRANSACTIONS)
    {
        Transaction* newTransaction = &transactions[(*count)++];

        /* Gán các giá trị */
        snprintf(newTransaction->sender, sizeof(newTransaction->sender), "%s", sender);
        newTransaction->timestamp = timestamp; /* Gán trực tiếp timestamp */
        snprintf(newTransaction->data, sizeof(newTransaction->data), "%s", data);

        /* Kiểm tra giao dịch hợp lệ */
        if (!verify_transaction(newTransaction))
        {
            fprintf(stderr, "Invalid transaction!\n");
            (*count)--; /* Giảm số lượng nếu giao dịch không hợp lệ */
        }
    }
    else
    {
        fprintf(stderr, "Transaction limit reached for this block!\n");
    }
}