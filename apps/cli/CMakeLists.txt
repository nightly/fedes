cmake_minimum_required (VERSION 3.8)

# FEDES Application/Executable
add_executable(fedes_cli "fedes.cpp" "models.cpp" "octree.cpp")

target_compile_features(fedes_cli PRIVATE cxx_std_23)
if(MSVC)
    target_compile_options(fedes_cli PUBLIC "/Zc:__cplusplus") 
endif()

# Dependencies
target_link_libraries(fedes_cli PRIVATE fedes)