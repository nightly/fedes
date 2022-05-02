cmake_minimum_required (VERSION 3.8)

add_library(thread_pool INTERFACE)

target_include_directories(thread_pool
    INTERFACE
        ${PROJECT_SOURCE_DIR}/external/thread-pool/src
)
