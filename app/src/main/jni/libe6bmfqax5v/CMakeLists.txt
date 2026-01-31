cmake_minimum_required(VERSION 3.18.1)

project(e6bmfqax5v)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2 -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -O2 -Wall -Wextra")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Source files
set(E6BM_SOURCES
    src/jni_interface.cpp
    src/crypto.cpp
    src/crypto_hash.cpp
    src/crypto_cbc.cpp
    src/utils.cpp
    src/init.cpp
    src/asset_crypto.cpp
)

# Create shared library
add_library(e6bmfqax5v SHARED ${E6BM_SOURCES})

# Link libraries
target_link_libraries(e6bmfqax5v
    log
    z
    dl
    m
)

# Set properties
set_target_properties(e6bmfqax5v PROPERTIES
    LIBRARY_OUTPUT_NAME "e6bmfqax5v"
    LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/exports.map"
)

# Strip in release mode
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET e6bmfqax5v POST_BUILD
        COMMAND ${CMAKE_STRIP} --strip-unneeded $<TARGET_FILE:e6bmfqax5v>
    )
endif()
