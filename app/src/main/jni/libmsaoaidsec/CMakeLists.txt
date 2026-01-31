cmake_minimum_required(VERSION 3.18.1)

project(msaoaidsec C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 -fvisibility=hidden")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2 -fvisibility=hidden")

add_library(msaoaidsec SHARED
    src/msaoaidsec.cpp
    src/types.cpp
    src/utils.cpp
    src/security.cpp
    src/device_id.cpp
    src/oaid_providers.cpp
    src/jni_interface.cpp
)

target_include_directories(msaoaidsec
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(msaoaidsec
    android
    log
    z
)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/exports.map)
    set_target_properties(msaoaidsec PROPERTIES
        LINK_FLAGS "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/exports.map"
    )
endif()

set_target_properties(msaoaidsec PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    OUTPUT_NAME msaoaidsec
)

install(TARGETS msaoaidsec
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include/msaoaidsec
    FILES_MATCHING PATTERN "*.h"
)
