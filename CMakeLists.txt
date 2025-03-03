cmake_minimum_required(VERSION 3.10)

# Project name and version
project(ATMTool VERSION 1.0)

# Set the C standard to C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_compile_options(
  "-Wall" "-Wpedantic" "-Wextra" "-O2"
)

# Create variables for source and include directories
set(SRC_DIR src/atm)
set(CRYPTO_DIR src/crypto)
set(INCLUDE_DIR include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add the executable with all the source files
add_executable(ATMTool
    ${SRC_DIR}/atm_decode.c
    ${SRC_DIR}/atm_encode.c
    ${SRC_DIR}/atm_header.c
    ${CRYPTO_DIR}/hc128.c
    src/main.c
)

target_include_directories(ATMTool PRIVATE
    ${INCLUDE_DIR}
)