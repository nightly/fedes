cmake_minimum_required (VERSION 3.8)

# Executable
add_executable(fedes_app "fedes.cpp")
target_link_libraries(fedes_app PRIVATE fedes)
target_compile_features(fedes_app PRIVATE cxx_std_20)
if(MSVC)
    target_compile_options(fedes_app PUBLIC "/Zc:__cplusplus") 
endif()