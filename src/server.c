#include "server.h"

#include "blockchain.h"
#include "coap.h"
#include "database.h"
#include "logger.h"
#include "utils.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static Blockchain* blockchain = NULL;
static sqlite3*    db         = NULL;

/* Function to run the server in a separate thread */
static void* run_server(void* arg)
{
    coap_start_server((const char*) arg);
    return NULL;
}

void create_and_add_block(Blockchain* blockchain, const char* sender, uint64_t timestamp, const char* data)
{
    Block* new_block = create_block(blockchain->tail->index + 1, blockchain->tail->hash);
    add_transaction(new_block->transactions, &new_block->transaction_count, sender, timestamp, data);
    add_block(blockchain, new_block);
}

void log_sensor_data(const char* url, SensorData* sensor_data)
{
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Retrieved sensor data for URL %s: Timestamp: %llu, Data: %s", url, (unsigned long long) sensor_data->timestamp, sensor_data->data);
    log_message(LOG_INFO, log_msg);

    /* Insert the new block into the blockchain and the database */
    create_and_add_block(blockchain, url, sensor_data->timestamp, (const char*) sensor_data->data);
    Block* lastBlock = blockchain->tail;

    insert_block_into_db(db, lastBlock->index, lastBlock->previous_hash, lastBlock->hash, lastBlock->transaction_count);
    for (int j = 0; j < lastBlock->transaction_count; j++)
    {
        insert_transaction_into_db(db,
                                   lastBlock->index,
                                   lastBlock->transactions[j].sender,
                                   lastBlock->transactions[j].timestamp,
                                   lastBlock->transactions[j].data,
                                   strlen(lastBlock->transactions[j].data));
        insert_data_into_db(db, lastBlock->transactions[j].timestamp, lastBlock->transactions[j].data, strlen(lastBlock->transactions[j].data));
    }
}

void start_blockchain_server()
{
    set_log_file("server.log");

    /* Create blockchain and genesis block */
    blockchain = create_blockchain();
    if (!blockchain)
    {
        fprintf(stderr, "Failed to create blockchain\n");
        return;
    }

    /* Create the genesis block */
    Block* genesis_block = create_block(0, "0123456789012345678901234567890123456789012345678901234567890123");
    if (!genesis_block)
    {
        fprintf(stderr, "Failed to create genesis block\n");
        free_blockchain(blockchain);
        return;
    }
    add_transaction(genesis_block->transactions, &genesis_block->transaction_count, "Alice", 0, "Hello, Bob!");
    add_block(blockchain, genesis_block);

    /* Start the CoAP server in a new thread */
    pthread_t   server_thread;
    const char* server_addr = "127.0.0.1:5683";
    pthread_create(&server_thread, NULL, run_server, (void*) server_addr);

    open_database(&db, "../blockchain.db");

    // Lưu con trỏ cơ sở dữ liệu vào biến toàn cục
    global_db = db;

    create_tables(db);

    const char* urls[] = {
        "sensor_data1/12345678/98765432",
        "sensor_data2/22345678/88765432",
        "sensor_data3/32345678/78765432",
        "sensor_data4/42345678/68765432",
        "sensor_data5/52345678/58765432",
    };

    while (1)
    {
        SensorData sensor_data;
        size_t     num_urls = sizeof(urls) / sizeof(urls[0]);

        for (size_t i = 0; i < num_urls; i++)
        {
            if (coap_retrieve_data(urls[i], &sensor_data))
            {
                log_sensor_data(urls[i], &sensor_data);
            }
        }

        sleep(2);
    }

    /* Verify blockchain */
    print_blockchain(blockchain);
    printf(verify_blockchain(blockchain) ? "Blockchain is valid!\n" : "Blockchain is invalid!\n");

    free_blockchain(blockchain);
    close_database(db);
    close_logger();
}
