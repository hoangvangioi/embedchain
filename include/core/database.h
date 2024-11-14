#ifndef DATABASE_H
#define DATABASE_H

#include "block.h"
#include "blockchain.h"
#include "sqlite3.h"

#include <pthread.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // Khai báo biến toàn cục mà không định nghĩa nó (extern)
    extern pthread_mutex_t db_mutex;
    extern sqlite3*        global_db;

    /* Function to open the database */
    int open_database(sqlite3** db, const char* filename);

    /* Function to close the database */
    void close_database(sqlite3* db);

    /* Function to create the SensorData table */
    void create_tables(sqlite3* db);

    int insert_block_into_db(sqlite3* db, int index, const char* previous_hash, const char* hash, int transaction_count);

    int insert_transaction_into_db(sqlite3* db, int block_index, const char* sender, long long timestamp, const void* data, int data_size);

    int insert_data_into_db(sqlite3* db, long long timestamp, const void* data, int data_size);

#ifdef __cplusplus
}
#endif

#endif /* DATABASE_H */
