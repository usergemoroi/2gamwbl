cmake_minimum_required(VERSION 3.18.1)

project(libclient_decompiled)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -DDEBUG")

# Source files
set(LIBCLIENT_SOURCES
    src/jni_interface.cpp
    src/init.cpp
    src/types.cpp
    src/render_engine.cpp
    src/game_detector.cpp
    src/network_client.cpp
    src/plugin_manager.cpp
    src/device_info.cpp
    src/anti_tamper.cpp
    src/client.cpp
    src/crypto_utils.cpp
    src/string_utils.cpp
    src/platform_specific.cpp
)

# Header files (for IDE support)
set(LIBCLIENT_HEADERS
    include/client.h
    include/types.h
    include/constants.h
    include/jni_bridge.h
    include/render_engine.h
    include/game_detector.h
    include/network_client.h
    include/plugin_manager.h
    include/device_info.h
    include/anti_tamper.h
    include/internal/crypto_utils.h
    include/internal/string_utils.h
    include/internal/platform_specific.h
)

# Create shared library
add_library(client_decompiled SHARED
    ${LIBCLIENT_SOURCES}
    ${LIBCLIENT_HEADERS}
)

# Include directories
target_include_directories(client_decompiled PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

# Find required libraries
find_library(LOG_LIB log)
find_library(ANDROID_LIB android)
find_library(EGL_LIB EGL)
find_library(GLES3_LIB GLESv3)
find_library(Z_LIB z)
find_library(DL_LIB dl)

# Link libraries
target_link_libraries(client_decompiled
    ${LOG_LIB}
    ${ANDROID_LIB}
    ${EGL_LIB}
    ${GLES3_LIB}
    ${Z_LIB}
    ${DL_LIB}
)

# Strip symbols in release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET client_decompiled POST_BUILD
        COMMAND ${CMAKE_STRIP} --strip-unneeded $<TARGET_FILE:client_decompiled>
    )
endif()

# Set output name
set_target_properties(client_decompiled PROPERTIES
    OUTPUT_NAME "client"
    VERSION "3.1.0"
    SOVERSION "3"
)

# Installation rules
install(TARGETS client_decompiled
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include/libclient
    FILES_MATCHING PATTERN "*.h"
)
