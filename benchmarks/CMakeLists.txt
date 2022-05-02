cmake_minimum_required (VERSION 3.8)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/external/googlebenchmark" "external/googlebenchmark")
set_property(DIRECTORY "${PROJECT_SOURCE_DIR}/external/googlebenchmark" APPEND
  PROPERTY COMPILE_DEFINITIONS "-DCMAKE_BUILD_TYPE=Release")

macro(package_add_benchmark BENCHNAME)
    add_executable(${BENCHNAME} ${ARGN})
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

package_add_benchmark("bench_octree_construct" "indexing/octree/construction/construct.cpp")
package_add_benchmark("bench_octree_large" "indexing/octree/construction/large.cpp")
package_add_benchmark("bench_octree_parallel_construct" "indexing/octree/construction/parallel.cpp")

package_add_benchmark("bench_octree_npm" "interpolations/octree/npm.cpp")
package_add_benchmark("bench_octree_fop" "interpolations/octree/fop.cpp")
package_add_benchmark("bench_octree_dmue" "interpolations/octree/dmue.cpp")
package_add_benchmark("bench_octree_esf" "interpolations/octree/esf.cpp")


package_add_benchmark("bench_experimental_1" "experimental/experimental_1.cpp")
package_add_benchmark("bench_experimental_2" "experimental/experimental_2.cpp")