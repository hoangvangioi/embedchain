cmake_minimum_required(VERSION 3.20)

project(blockchain VERSION 1.0 LANGUAGES C CXX)

# Output system and project details
message(STATUS "[${PROJECT_NAME}] CMake version ${CMAKE_VERSION}")
message(STATUS "[${PROJECT_NAME}] System ${CMAKE_SYSTEM_NAME}")
message(STATUS "[${PROJECT_NAME}] Processor ${CMAKE_SYSTEM_PROCESSOR}")

set(CMAKE_BUILD_TYPE Release)

# Enable warnings in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    elseif(MSVC)
        add_compile_options(/W4 /WX)
    endif()
endif()

# Enable compilation command export for use with clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Set Qt-related configuration
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

# Set C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Rust project path
set(COAP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/coap)

# External Rust project setup
include(ExternalProject)
ExternalProject_Add(
    coap_lib
    SOURCE_DIR ${COAP_SOURCE_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cargo build --release --manifest-path=${COAP_SOURCE_DIR}/Cargo.toml
    INSTALL_COMMAND ""
)

# Define the Rust library as an imported target
add_library(coap SHARED IMPORTED)
set_target_properties(coap PROPERTIES
    IMPORTED_LOCATION "${COAP_SOURCE_DIR}/target/release/libcoap.a"
)

# Add subdirectories
add_subdirectory(tests)

# Find required Qt6 components
find_package(Qt6 COMPONENTS
    Gui Svg Core Charts Widgets SvgWidgets REQUIRED
)

# Collect source and header files
file(GLOB_RECURSE SOURCES "main.cpp" "src/*.c" "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# Exclude CMake-related files from sources
list(FILTER SOURCES EXCLUDE REGEX "CMakeFiles|tests")

# Define resource files
set(RESOURCES resources/resources.qrc)

# Process headers for MOC
qt6_wrap_cpp(MOC_SOURCES ${HEADERS})

# Add the executable target
if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${MOC_SOURCES} ${RESOURCES})
elseif(APPLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} ${MOC_SOURCES} ${RESOURCES})
elseif(UNIX)
    add_executable(${PROJECT_NAME} ${SOURCES} ${MOC_SOURCES} ${RESOURCES})
else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()

# Link libraries to the target
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Gui Qt6::Svg Qt6::Core Qt6::Charts Qt6::Widgets Qt6::SvgWidgets
    coap pthread m
    ${CMAKE_SOURCE_DIR}/libs/libsqlite3.a
    ${CMAKE_SOURCE_DIR}/libs/libblake3.a
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pages
    ${CMAKE_CURRENT_SOURCE_DIR}/include/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/include/blake3
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sqlite3
)

# Link the Rust library as a dependency
add_dependencies(${PROJECT_NAME} coap_lib)

# Custom target to run the application
add_custom_target(run
    COMMAND ./${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
)

# Custom target to run tests
add_custom_target(run_tests
    DEPENDS test
    COMMENT "Running client and server from tests"
)

# Set installation directories
set(CMAKE_INSTALL_PREFIX "./test" CACHE PATH "Installation prefix" FORCE)
set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "Executable directory" FORCE)
set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "Library directory" FORCE)
set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "Header file directory" FORCE)
set(CMAKE_INSTALL_DATADIR "share/my_project/data" CACHE PATH "Data directory" FORCE)
set(CMAKE_INSTALL_SYSCONFDIR "etc/my_project" CACHE PATH "Configuration directory" FORCE)
set(CMAKE_INSTALL_DOCDIR "share/doc/my_project" CACHE PATH "Documentation directory" FORCE)
set(CMAKE_INSTALL_MANDIR "share/man/man1" CACHE PATH "Man page directory" FORCE)

# Specify installation rules
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES libs/libblake3.a DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES my_config.cfg DESTINATION ${CMAKE_INSTALL_SYSCONFDIR})
