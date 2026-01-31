# CMakeLists.txt for GameBlaster Pro v3.1 Native Libraries
# Decompiled and reconstructed from original APK

cmake_minimum_required(VERSION 3.10.2)

project(gameblaster-native)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")

# Find required packages
find_library(log-lib log)
find_library(android-lib android)
find_library(EGL-lib EGL)
find_library(GLESv3-lib GLESv3)
find_library(z-lib z)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/libclient
    ${CMAKE_CURRENT_SOURCE_DIR}/lib2f8c0b3257fcc345
    ${CMAKE_CURRENT_SOURCE_DIR}/libe6bmfqax5v
    ${CMAKE_CURRENT_SOURCE_DIR}/libmsaoaidsec
)

# libclient.so - Main client library
add_library(client SHARED
    libclient/client.cpp
)

target_link_libraries(client
    ${log-lib}
    ${android-lib}
    ${EGL-lib}
    ${GLESv3-lib}
    ${z-lib}
)

# lib2f8c0b3257fcc345.so - Burri encrypted loader
add_library(2f8c0b3257fcc345 SHARED
    lib2f8c0b3257fcc345/burri_loader.cpp
)

target_link_libraries(2f8c0b3257fcc345
    ${log-lib}
    ${android-lib}
)

# libe6bmfqax5v.so - Auxiliary crypto library
add_library(e6bmfqax5v SHARED
    libe6bmfqax5v/auxiliary.cpp
)

target_link_libraries(e6bmfqax5v
    ${log-lib}
    ${z-lib}
    crypto
    ssl
)

# libmsaoaidsec.so - Security and device ID library
add_library(msaoaidsec SHARED
    libmsaoaidsec/security.cpp
)

target_link_libraries(msaoaidsec
    ${log-lib}
    ${android-lib}
)

# Set output names to match original library names
set_target_properties(client PROPERTIES OUTPUT_NAME "client")
set_target_properties(2f8c0b3257fcc345 PROPERTIES OUTPUT_NAME "2f8c0b3257fcc345")
set_target_properties(e6bmfqax5v PROPERTIES OUTPUT_NAME "e6bmfqax5v")
set_target_properties(msaoaidsec PROPERTIES OUTPUT_NAME "msaoaidsec")

# Strip symbols in release builds
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--exclude-libs,ALL")
endif()
