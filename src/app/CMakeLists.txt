cmake_minimum_required (VERSION 3.8)

# FEDES Application/Executable
add_executable(fedes_app "fedes.cpp" "models.cpp" "octree.cpp")

target_compile_features(fedes_app PRIVATE cxx_std_20)
if(MSVC)
    target_compile_options(fedes_app PUBLIC "/Zc:__cplusplus") 
endif()

# Dependencies
target_link_libraries(fedes_app PRIVATE fedes)

# Preprocessor definitions
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND FEDES_RUN_TESTS)
    target_compile_definitions(fedes_app PRIVATE FEDES_BENCHMARKING=1)
endif()