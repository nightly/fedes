cmake_minimum_required (VERSION 3.8)

add_subdirectory("${PROJECT_SOURCE_DIR}/external/googlebenchmark" "external/googlebenchmark")

macro(package_add_benchmark BENCHNAME)
    add_executable(${BENCHNAME} ${ARGN})

    target_compile_features(${BENCHNAME}  PRIVATE cxx_std_20)
    if(MSVC)
        target_compile_options(${BENCHNAME} PUBLIC "/Zc:__cplusplus") 
    endif()

    target_link_libraries(${BENCHNAME} 
    PRIVATE 
        fedes
        benchmark::benchmark)

    set_target_properties(${TESTNAME} PROPERTIES FOLDER benchmarks)
endmacro()