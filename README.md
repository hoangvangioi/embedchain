# EmbedChain

## Introduction

EmbedChain is a demo application showcasing the use of blockchain technology to store and transmit data from sensor devices. This includes data such as location, temperature, humidity, pressure, air quality, and other environmental metrics. The sensor data is encrypted using ChaCha20, sent over the COAP protocol, and encoded with Protocol Buffers for security and efficiency. The data is hashed using the Blake3 algorithm to ensure integrity, while SQLite3 is used for local storage. The user interface is built with Qt6 and C++.

## Key Features

- **Blockchain Storage**: Sensor data is stored in a blockchain network to ensure data integrity and transparency.
- **ChaCha20 Encryption**: Sensor data is encrypted with the ChaCha20 algorithm to protect sensitive information during transmission.
- **COAP & Protocol Buffers**: Data is sent over the COAP protocol and encoded using Protocol Buffers for secure and efficient data transmission.
- **Blake3 Hashing**: Each data packet is hashed using the Blake3 algorithm to verify data integrity.
- **SQLite3 Storage**: Data is stored in an SQLite3 database, allowing for easy retrieval and management.
- **Qt6 User Interface**: A user-friendly interface is built with Qt6 and C++ to interact with the system easily.

## Build and Run

1. Clone the repository:
   ```bash
   git clone https://github.com/hoangvangioi/embedchain.git
   cd embedchain
   ```

2. Build:
   ```bash
    mkdir -p build
    cd build
    cmake ..
    make
   ```

3. Run the application:
   ```bash
   ./embedchain
   ```
