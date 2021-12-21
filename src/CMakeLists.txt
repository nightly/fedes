cmake_minimum_required (VERSION 3.8)

# FEDES library
add_library(fedes STATIC "data/vector3.cpp" "indexing/octree/octree.cpp" "indexing/octree/octant.cpp" "interop/dynamic_array.cpp"  "instrumentation/timer.cpp")
target_compile_features(fedes PRIVATE cxx_std_20)
if(MSVC)
    target_compile_options(fedes PUBLIC "/Zc:__cplusplus") # Reports __cplusplus version correctly for external libraries checking it
endif()

target_include_directories(fedes
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

# Dependencies 
add_subdirectory("${PROJECT_SOURCE_DIR}/external/matchit" "external/matchit")
target_link_libraries(fedes PRIVATE matchit)
