#ifndef COAP_H
#define COAP_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* Định nghĩa cấu trúc dữ liệu tương ứng với Rust */
    typedef struct
    {
        uint64_t timestamp;
        uint8_t* data;
    } SensorData;

    /* Cấu trúc để lưu trữ thông tin vị trí */
    typedef struct
    {
        const char* url;
        uint32_t    latitude;
        uint32_t    longitude;
    } SensorLocation;

/* Định nghĩa lại tên hàm */
#define coap_send_data send_sensor_data
#define coap_start_server start_server
#define coap_free_data free_sensor_data
#define coap_retrieve_data retrieve_sensor_data

    /* Khai báo các hàm từ Rust */
    void send_sensor_data(const char* url, unsigned long long timestamp, uint8_t* data);
    void start_server(const char* addr);
    void free_sensor_data(const SensorData* data);
    int  retrieve_sensor_data(const char* url, SensorData* data);

#ifdef __cplusplus
}
#endif

#endif /* COAP_H */
