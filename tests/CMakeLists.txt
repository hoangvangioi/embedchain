cmake_minimum_required(VERSION 3.30)
project(sensorclient)

set(CLIENT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/core/chacha20.c
    client.c
)

# Create the server and client executables
add_executable(client ${CLIENT_SOURCES})

# Link libraries to the executables
target_link_libraries(client PRIVATE pthread coap m)

# Specify include directories for both server and client
target_include_directories(client PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/core
)

# Define a custom target to run client
add_custom_target(test
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/run_dir

    # Run the client
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} ./client > ${CMAKE_BINARY_DIR}/run_dir/output2.log 2>&1
    DEPENDS client
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running client"
)
