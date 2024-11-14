#ifndef UTILS_H
#define UTILS_H

#include "blockchain.h"
#include "chacha20.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void saveBlockchainToFile(Blockchain* blockchain);

    void loadBlockchainFromFile(Blockchain* blockchain);

    /* Hàm tạo byte ngẫu nhiên */
    void generate_random_bytes(uint8_t* buffer, size_t size);

    /* Hàm mã hóa dữ liệu */
    void encrypt_data(struct chacha20_context* ctx, uint8_t* data, size_t length);

    /* Hàm giải mã dữ liệu */
    void decrypt_data(struct chacha20_context* ctx, uint8_t* data, size_t length);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_H */
