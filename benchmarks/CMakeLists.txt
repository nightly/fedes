cmake_minimum_required (VERSION 3.8)

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

package_add_benchmark("bench_octree_npm_m1" "interpolations/octree/npm/m1.cpp")
package_add_benchmark("bench_octree_npm_m2" "interpolations/octree/npm/m2.cpp")
package_add_benchmark("bench_octree_npm_m3" "interpolations/octree/npm/m3.cpp")
package_add_benchmark("bench_octree_npm_m4" "interpolations/octree/npm/m4.cpp")

package_add_benchmark("bench_octree_fop_m1" "interpolations/octree/fop/m1.cpp")
package_add_benchmark("bench_octree_fop_m2" "interpolations/octree/fop/m2.cpp")
package_add_benchmark("bench_octree_fop_m3" "interpolations/octree/fop/m3.cpp")
package_add_benchmark("bench_octree_fop_m4" "interpolations/octree/fop/m4.cpp")

package_add_benchmark("bench_octree_dmue_m1" "interpolations/octree/dmue/m1.cpp")
package_add_benchmark("bench_octree_dmue_m2" "interpolations/octree/dmue/m2.cpp")
package_add_benchmark("bench_octree_dmue_m3" "interpolations/octree/dmue/m3.cpp")
package_add_benchmark("bench_octree_dmue_m4" "interpolations/octree/dmue/m4.cpp")


package_add_benchmark("bench_octree_esf_m1" "interpolations/octree/esf/m1.cpp")
package_add_benchmark("bench_octree_esf_m2" "interpolations/octree/esf/m2.cpp")
package_add_benchmark("bench_octree_esf_m3" "interpolations/octree/esf/m3.cpp")
package_add_benchmark("bench_octree_esf_m4" "interpolations/octree/esf/m4.cpp")

# ============
#   Other
# ===========

package_add_benchmark("bench_experimental_1" "experimental/experimental_1.cpp")
package_add_benchmark("bench_experimental_2" "experimental/experimental_2.cpp")