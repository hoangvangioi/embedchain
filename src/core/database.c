#include "database.h"

#include "sqlite3.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Khai báo mutex toàn cục
pthread_mutex_t db_mutex  = PTHREAD_MUTEX_INITIALIZER;
sqlite3*        global_db = NULL; // Cơ sở dữ liệu toàn cục

// Hàm mở kết nối tới cơ sở dữ liệu SQLite
int open_database(sqlite3** db, const char* filename)
{
    if (sqlite3_open(filename, db) != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        return 1;
    }
    return 0;
}

// Hàm đóng kết nối tới cơ sở dữ liệu SQLite
void close_database(sqlite3* db)
{
    sqlite3_close(db);
}

// Hàm tạo bảng nếu chưa tồn tại
void create_tables(sqlite3* db)
{
    const char* sql[] = {
        "CREATE TABLE IF NOT EXISTS blocks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "block_index INTEGER NOT NULL, "
        "previous_hash TEXT NOT NULL, "
        "hash TEXT NOT NULL, "
        "transaction_count INTEGER NOT NULL);",

        "CREATE TABLE IF NOT EXISTS transactions ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "block_index INTEGER NOT NULL, "
        "sender TEXT NOT NULL, "
        "timestamp BIGINT NOT NULL, "
        "data BLOB NOT NULL, "
        "FOREIGN KEY (block_index) REFERENCES blocks (block_index) ON DELETE CASCADE);",

        "CREATE TABLE IF NOT EXISTS data ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "timestamp BIGINT NOT NULL, "
        "latitude TEXT, "
        "longitude TEXT, "
        "temperature TEXT, "
        "humidity TEXT, "
        "pressure TEXT, "
        "co2 TEXT, "
        "no2 TEXT, "
        "o3 TEXT);"};

    for (int i = 0; i < sizeof(sql) / sizeof(sql[0]); i++)
    {
        char* err_msg = NULL;
        if (sqlite3_exec(db, sql[i], NULL, NULL, &err_msg) != SQLITE_OK)
        {
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
        }
    }
}

// Hàm chuẩn bị và thực thi câu lệnh SQL
static int execute_sql(sqlite3* db, const char* sql, sqlite3_stmt** stmt)
{
    pthread_mutex_lock(&db_mutex);

    if (sqlite3_prepare_v3(db, sql, -1, SQLITE_PREPARE_PERSISTENT, stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    pthread_mutex_unlock(&db_mutex);
    return 0;
}

// Hàm chèn một bản ghi vào bảng `blocks`
int insert_block_into_db(sqlite3* db, int index, const char* previous_hash, const char* hash, int transaction_count)
{
    const char*   sql = "INSERT INTO blocks (block_index, previous_hash, hash, transaction_count) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (execute_sql(db, sql, &stmt) != 0) return -1;

    sqlite3_bind_int(stmt, 1, index);
    sqlite3_bind_text(stmt, 2, previous_hash, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, hash, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, transaction_count);

    pthread_mutex_lock(&db_mutex);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);

    return 0;
}

// Hàm chèn một bản ghi vào bảng `transactions`
int insert_transaction_into_db(sqlite3* db, int block_index, const char* sender, long long timestamp, const void* data, int data_size)
{
    const char*   sql = "INSERT INTO transactions (block_index, sender, timestamp, data) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (execute_sql(db, sql, &stmt) != 0) return -1;

    sqlite3_bind_int(stmt, 1, block_index);
    sqlite3_bind_text(stmt, 2, sender, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 3, timestamp);
    sqlite3_bind_blob(stmt, 4, data, data_size, SQLITE_STATIC);

    pthread_mutex_lock(&db_mutex);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);

    return 0;
}

// Hàm chèn một bản ghi vào bảng `data`
int insert_data_into_db(sqlite3* db, long long timestamp, const void* data, int data_size)
{
    const char*   sql = "INSERT INTO data (timestamp, latitude, longitude, temperature, humidity, pressure, co2, no2, o3) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;

    if (execute_sql(db, sql, &stmt) != 0) return -1;

    // Giải mã dữ liệu
    uint8_t key[33]   = "0123456789abcdef0123456789abcdef";
    uint8_t nonce[12] = {0};

    struct chacha20_context ctx;
    chacha20_init_context(&ctx, key, nonce, 0);
    decrypt_data(&ctx, data, data_size); // Giải mã dữ liệu

    char* input_copy = strdup(data);
    if (!input_copy)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        sqlite3_finalize(stmt);
        return 1;
    }

    // Tách chuỗi theo dấu "|"
    char* token     = strtok(input_copy, "|");
    int   col_index = 1;

    while (token != NULL && col_index <= 8)
    {
        sqlite3_bind_text(stmt, col_index + 1, token, -1, SQLITE_STATIC);
        token = strtok(NULL, "|");
        col_index++;
    }

    sqlite3_bind_int64(stmt, 1, timestamp);

    // Gán NULL cho các cột còn thiếu
    while (col_index <= 8)
    {
        sqlite3_bind_null(stmt, col_index + 1);
        col_index++;
    }

    pthread_mutex_lock(&db_mutex);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    }

    free(input_copy);
    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);

    return 0;
}
