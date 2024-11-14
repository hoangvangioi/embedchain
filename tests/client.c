#include "chacha20.h"
#include "coap.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void encrypt_data(struct chacha20_context* ctx, uint8_t* data, size_t length)
{
    chacha20_xor(ctx, data, length);
}

// Function to log messages to a file
void log_message(uint8_t* message)
{
    FILE* log_file = fopen("client.log", "a");
    if (log_file)
    {
        fprintf(log_file, "%s\n", message);
        fclose(log_file);
    }
    else
    {
        perror("Failed to open log file");
    }
}

uint8_t* create_data_string(uint32_t latitude, uint32_t longitude, uint32_t temperature, uint32_t humidity, uint32_t co2, uint32_t no2, uint32_t o3, uint32_t pressure)
{
    // Tính kích thước cần thiết cho chuỗi
    size_t size = snprintf(NULL, 0, "%u|%u|%u|%u|%u|%u|%u|%u", latitude, longitude, temperature, humidity, co2, no2, o3, pressure) + 1;

    uint8_t* buffer = (uint8_t*) malloc(size);
    if (buffer == NULL)
    {
        perror("Failed to allocate memory");
        return NULL;
    }

    // Tạo chuỗi và kiểm tra kết quả
    if (snprintf((char*) buffer, size, "%u|%u|%u|%u|%u|%u|%u|%u", latitude, longitude, temperature, humidity, co2, no2, o3, pressure) < 0)
    {
        free(buffer); // Giải phóng bộ nhớ nếu có lỗi
        return NULL;
    }

    return buffer;
}

int main()
{
    SensorLocation locations[] = {{"coap://127.0.0.1:5683/sensor_data1", 12345678, 98765432},
                                  {"coap://127.0.0.1:5683/sensor_data2", 22345678, 88765432},
                                  {"coap://127.0.0.1:5683/sensor_data3", 32345678, 78765432},
                                  {"coap://127.0.0.1:5683/sensor_data4", 42345678, 68765432},
                                  {"coap://127.0.0.1:5683/sensor_data5", 52345678, 58765432}};

    size_t num_locations = sizeof(locations) / sizeof(locations[0]);
    srand((unsigned) time(NULL)); // Initialize random seed

    uint8_t key[32] = "0123456789abcdef0123456789abcdef"; // Proper key size for ChaCha20

    while (1)
    {
        // Send data to each location
        for (size_t i = 0; i < num_locations; i++)
        {
            uint64_t timestamp = (uint64_t) time(NULL) * 1000; // Timestamp

            // Generate random values
            uint16_t co2         = rand() % 200 + 400;  // From 400 to 600
            uint8_t  no2         = rand() % 100;        // From 0 to 100
            uint8_t  o3          = rand() % 100;        // From 0 to 100
            uint8_t  humidity    = rand() % 101;        // From 0 to 100
            uint8_t  temperature = rand() % 41 + 10;    // From 10 to 50
            uint16_t pressure    = rand() % 100 + 1000; // From 1000 to 1100

            char url[56];
            snprintf(url, sizeof(url), "%s/%u/%u", locations[i].url, locations[i].latitude, locations[i].longitude);

            uint8_t* data = create_data_string(locations[i].latitude, locations[i].longitude, temperature, humidity, co2, no2, o3, pressure);
            if (!data) return 1;

            size_t data_length = strlen((char*) data); // Corrected length calculation

            uint8_t nonce[12] = {0}; // Khởi tạo nonce

            // Initialize ChaCha20 context
            struct chacha20_context ctx;
            chacha20_init_context(&ctx, key, nonce, 0); // Counter starts from 0

            // Encrypt the data
            encrypt_data(&ctx, data, data_length); // Use the length of the string

            // Call the Rust function (assumes coap_send_data is defined elsewhere)
            coap_send_data(url, timestamp, data); // Pass the URL, timestamp, and data
            printf("Sent data to %s\n", url);

            free(data);
        }

        sleep(2);
    }

    return 0;
}
