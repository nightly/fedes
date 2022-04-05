cmake_minimum_required (VERSION 3.8)
project ("fedes" VERSION 0 LANGUAGES CXX)

option(FEDES_RUN_TESTS "Run tests" ON)

# Library
add_subdirectory(src/fedes)

# Tests
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND FEDES_RUN_TESTS)
    include(CTest)
    enable_testing()
    include(GoogleTest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(tests)
endif()

# Application
add_subdirectory(apps/cli)

# Benchmarks
add_subdirectory(benchmarks)

set_target_properties(fedes fedes_cli
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
)