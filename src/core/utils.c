#include "utils.h"

#include "block.h"

#include <stdio.h>
#include <stdlib.h>

void saveBlockchainToFile(Blockchain* blockchain)
{
    FILE* file = fopen("blockchain.txt", "w");
    if (file)
    {
        Block* current = blockchain->head;
        while (current != NULL)
        {
            fprintf(file, "Block #%d\n", current->index);
            fprintf(file, "Previous Hash: %s\n", current->previous_hash);
            for (int i = 0; i < current->transaction_count; i++)
            {
                Transaction* tx = &current->transactions[i];
                fprintf(file, "  %s -> %lu: %s\n", tx->sender, tx->timestamp, tx->data);
            }
            fprintf(file, "Hash: %s\n\n", current->hash);
            current = current->next;
        }
        fclose(file);
    }
    else
    {
        printf("Error saving blockchain to file.\n");
    }
}

void loadBlockchainFromFile(Blockchain* blockchain)
{
    FILE* file = fopen("blockchain.txt", "r");
    if (!file)
    {
        printf("Error loading blockchain from file.\n");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "Block"))
        {
            int index;
            sscanf(line, "Block #%d", &index);
            Block* new_block = (Block*) malloc(sizeof(Block));
            if (!new_block)
            {
                printf("Memory allocation error.\n");
                fclose(file);
                return;
            }
            new_block->index = index;
            strcpy(new_block->previous_hash, "0"); /* Giá trị tạm thời cho previous hash */
            new_block->transaction_count = 0;

            while (fgets(line, sizeof(line), file) && !strstr(line, "Hash:"))
            {
                if (strstr(line, "->"))
                {
                    Transaction* tx = &new_block->transactions[new_block->transaction_count++];
                    sscanf(line, "  %s -> %lu: %[^\n]", tx->sender, &tx->timestamp, tx->data);
                }
            }
            sscanf(line, "Hash: %s", new_block->hash);
            new_block->next = NULL; /* Đảm bảo con trỏ next được khởi tạo */

            add_block(blockchain, new_block);
        }
    }
    fclose(file);
}

/* Lưu blockchain vào tệp */
/* saveBlockchainToFile(blockchain); */

/* Tải blockchain từ tệp */
/* Blockchain* loadedBlockchain = create_blockchain(); */
/* loadBlockchainFromFile(loadedBlockchain); */
/* printf("Loaded Blockchain:\n"); */
/* print_blockchain(loadedBlockchain); */

/* Giải phóng bộ nhớ */
/* free_blockchain(blockchain); */
/* free_blockchain(loadedBlockchain); */

void generate_random_bytes(uint8_t* buffer, size_t size)
{
    FILE* fp = fopen("/dev/urandom", "rb");
    if (fp == NULL)
    {
        perror("Error opening /dev/urandom");
        exit(EXIT_FAILURE);
    }

    if (fread(buffer, 1, size, fp) != size)
    {
        perror("Error reading from /dev/urandom");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);
}

void encrypt_data(struct chacha20_context* ctx, uint8_t* data, size_t length)
{
    chacha20_xor(ctx, data, length);
}

void decrypt_data(struct chacha20_context* ctx, uint8_t* data, size_t length)
{
    chacha20_xor(ctx, data, length);
}
