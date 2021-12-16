cmake_minimum_required (VERSION 3.8)

project ("fedes" VERSION 0 LANGUAGES CXX)
option(FEDES_RUN_TESTS "Run tests" ON)

add_library(fedes "src/fedes.cpp" "src/data/vector3.cpp" "src/indexing/octree.cpp" "src/indexing/octant.cpp" "src/interop/dynamic_array.cpp"  "src/instrumentation/timer.cpp")
target_compile_features(fedes PRIVATE cxx_std_20)

if(MSVC)
    target_compile_options(fedes PUBLIC "/Zc:__cplusplus") # Reports __cplusplus version correctly for external libraries checking it
endif()

target_include_directories(fedes
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory("${PROJECT_SOURCE_DIR}/external/patterns" "external/patterns")
target_link_libraries(fedes mpark_patterns)

# Tests
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND FEDES_RUN_TESTS)
    enable_testing() 
    include(GoogleTest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(tests)
endif()