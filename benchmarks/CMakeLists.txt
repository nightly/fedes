cmake_minimum_required (VERSION 3.22)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/external/googlebenchmark" "external/googlebenchmark")
set_property(DIRECTORY "${PROJECT_SOURCE_DIR}/external/googlebenchmark" APPEND
  PROPERTY COMPILE_DEFINITIONS "-DCMAKE_BUILD_TYPE=Release")

macro(package_add_benchmark BENCHNAME)
    add_executable(${BENCHNAME} ${ARGN} "indexing/octree/construction/m4.cpp")
    target_compile_features(${BENCHNAME}  PRIVATE cxx_std_23)

    target_link_libraries(${BENCHNAME} PRIVATE 
        fedes
        benchmark::benchmark
        benchmark::benchmark_main)

    set_target_properties(${BENCHNAME} PROPERTIES 
        FOLDER benchmarks
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
        ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}")
endmacro()

# Benchmark Suite

# ==================
#   Index Creation
# ==================

package_add_benchmark("bench_octree_m1" "indexing/octree/construction/m1.cpp")
package_add_benchmark("bench_octree_m2" "indexing/octree/construction/m2.cpp")
package_add_benchmark("bench_octree_m3" "indexing/octree/construction/m3.cpp")
package_add_benchmark("bench_octree_m4" "indexing/octree/construction/m4.cpp")

# ==================
#   Interpolations
# ==================

package_add_benchmark("bench_octree_interpolations" "interpolations/octree/octree.cpp")